/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_nonlinear_material.hpp"
#include <stdexcept>
#include <cmath>
#include <cstring>
#include <cassert>

p6::NonlinearMaterial::NonlinearMaterial(const String name, const String formula)
{
	if (name == "") throw std::runtime_error("Name of material cannot be empty");
	_name = name;
	_formula = formula;
	_last_strain = nan("");

	//Prinary check (check illegal symbols)
	for (uint i = 0; i < formula.size(); i++)
	{
		char c = formula[i];
		if (!(('0' <= c && c <= '9')
		|| ('a' <= c && c <= 'z')
		|| strchr("()+-*/. ", c) != nullptr))
			throw std::runtime_error("NonlinearMaterial::parse() : illegal symbol");
	}

	//Parsing and primary check (check s)
	std::vector<Word> left;
	const char *p = &formula[0];
	while (p <= &formula.back())
	{
		if ('0' <= *p && *p <= '9')
		{
			char *end;
			Word newword;
			newword.type = Word::Type::REAL;
			newword.number = strtod(p, &end);
			p = end;
			left.push_back(newword);
		}
		else if ('a' <= *p && *p <= 'z')
		{
			uint length = 0;
			while ('a' <= p[length] && p[length] <= 'z') length++;
			Word newword;
			if (length == 1 && memcmp(p, "s", 1) == 0) newword.type = Word::Type::STRAIN;
			else if (length == 3 && memcmp(p, "sin", 3) == 0) newword.type = Word::Type::SIN;
			else if (length == 3 && memcmp(p, "cos", 3) == 0) newword.type = Word::Type::COS;
			else if (length == 2 && memcmp(p, "ln", 2) == 0) newword.type = Word::Type::LN;
			else if (length == 3 && memcmp(p, "exp", 3) == 0) newword.type = Word::Type::EXP;
			else throw std::runtime_error("NonlinearMaterial::parse(): invalid identifier\n");
			p += length;
			left.push_back(newword);
		}
		else if (strchr("+-*/()", *p) != nullptr)
		{
			Word newword;
			switch (*p)
			{
				case '+': newword.type = Word::Type::ADD; break;
				case '-': newword.type = Word::Type::SUB; break;
				case '*': newword.type = Word::Type::MUL; break;
				case '/': newword.type = Word::Type::DIV; break;
				case '(': newword.type = Word::Type::OPEN; break;
				default : newword.type = Word::Type::CLOSE; break;
			}
			left.push_back(newword);
			p++;
		}
		else if (*p == ' ') p++;
		else throw std::runtime_error("NonlinearMaterial::parse(): illegal word begin\n");
	}

	//Secondary check (ckeck sequent operators and identifiers, setermining unar minus)
	for (uint i = 0; i <= left.size(); i++)
	{
		//Need numbers
		if (i == 0 ||
			left[i - 1].type == Word::Type::ADD ||
			left[i - 1].type == Word::Type::SUB ||
			left[i - 1].type == Word::Type::MUL ||
			left[i - 1].type == Word::Type::DIV ||
			left[i - 1].type == Word::Type::NEG ||
			left[i - 1].type == Word::Type::OPEN)
		{
			if (i < left.size())
			{
				if (left[i].type == Word::Type::SUB) left[i].type = Word::Type::NEG;
				if (left[i].type == Word::Type::NEG			||
					left[i].type == Word::Type::SIN			||
					left[i].type == Word::Type::COS			||
					left[i].type == Word::Type::LN			||
					left[i].type == Word::Type::EXP			||
					left[i].type == Word::Type::REAL			||
					left[i].type == Word::Type::STRAIN		||
					left[i].type == Word::Type::OPEN) continue;
			}
			throw std::runtime_error("NonlinearMaterial::parse(): illegal operator usage\n");
		}
		//Need operators
		else if (
			left[i - 1].type == Word::Type::REAL		||
			left[i - 1].type == Word::Type::STRAIN	||
			left[i - 1].type == Word::Type::CLOSE)
		{
			if (i == left.size() ||
				left[i].type == Word::Type::ADD ||
				left[i].type == Word::Type::SUB ||
				left[i].type == Word::Type::MUL ||
				left[i].type == Word::Type::DIV ||
				left[i].type == Word::Type::CLOSE) continue;
			throw std::runtime_error("NonlinearMaterial::parse(): illegal number usage\n");
		}
		//Need opening bracket
		else
		{
			if (i < left.size() && left[i].type == Word::Type::OPEN) continue;
			throw std::runtime_error("NonlinearMaterial::parse(): illegal function usage\n");
		}
	}

	//Third check (counting brackets)
	uint bracketcount = 0;
	for (uint i = 0; i < left.size(); i++)
	{
		if (left[i].type == Word::Type::OPEN)
		{
			bracketcount++;
		}
		else if (left[i].type == Word::Type::CLOSE)
		{
			if (bracketcount-- == 0) break;
		}
	}
	if (bracketcount != 0)
		throw std::runtime_error("NonlinearMaterial::parse(): illegal bracket usage\n");

	//Transforming to inverse polish notation (pulling right)
	std::vector<Word::Type> stack;
	while (true)
	{
		//Begin, throws everyting out of stack or quits
		if (left.empty())
		{
			if (stack.empty()) break;
			else
			{
				_operations.push_back(static_cast<Operation>(stack.back()));
				stack.pop_back();
			}
		}
		//Opening bracket, throws everyting out of stack or terminates closing bracket
		else if (left.back().type == Word::Type::OPEN)
		{
			if (stack.back() == Word::Type::CLOSE)
			{
				left.pop_back();
				stack.pop_back();
			}
			else
			{
				_operations.push_back(static_cast<Operation>(stack.back()));
				stack.pop_back();
			}
		}
		//Closing bracket and */ operators, go to stack
		else if (left.back().type == Word::Type::CLOSE	||
			left.back().type == Word::Type::MUL			||
			left.back().type == Word::Type::DIV)
		{
			stack.push_back(left.back().type);
			left.pop_back();
		}
		//+- operators, throw */ operators out of stack or go to stack
		else if (left.back().type == Word::Type::ADD || left.back().type == Word::Type::SUB)
		{
			if (!stack.empty() && (stack.back() == Word::Type::MUL || stack.back() == Word::Type::DIV))
			{
				_operations.push_back(static_cast<Operation>(stack.back()));
				stack.pop_back();
			}
			else
			{
				stack.push_back(left.back().type);
				left.pop_back();
			}
		}
		//Functions go straight right
		else if (left.back().type == Word::Type::STRAIN	||
			left.back().type == Word::Type::NEG			||
			left.back().type == Word::Type::SIN			||
			left.back().type == Word::Type::COS			|| 
			left.back().type == Word::Type::LN			|| 
			left.back().type == Word::Type::EXP)
		{
			_operations.push_back(static_cast<Operation>(left.back().type));
			left.pop_back();
		}
		//Real if kind of function too, goes right
		else
		{
			_operations.push_back(Operation::PUTR);
			_operations.resize(_operations.size()+sizeof(real));
			memcpy(&_operations[_operations.size()-sizeof(real)], &left.back().number, sizeof(real));
			left.pop_back();
		}
	}
}

p6::String p6::NonlinearMaterial::formula() const noexcept
{
	return _formula;
}

p6::Material::Type p6::NonlinearMaterial::type() const noexcept
{
	return p6::Material::Type::nonlinear;
}

p6::real p6::NonlinearMaterial::stress(real strain) const noexcept
{
	if (strain != _last_strain)
	{
		_last_strain = strain;
		_calculate();
	}
	return _last_stress;
}

p6::real p6::NonlinearMaterial::derivative(real strain) const noexcept
{
	if (strain != _last_strain)
	{
		_last_strain = strain;
		_calculate();
	}
	return _last_derivative;
}

void p6::NonlinearMaterial::_calculate() const noexcept
{
	std::vector<StackElement> stack;

	//Execute
	uint i = 0;
	while (i < _operations.size())
	{
		switch (_operations[i++])
		{
		case Operation::PUTR:
			stack.resize(stack.size() + 1);
			memcpy(&stack.back().value, &_operations[i], sizeof(real));
			stack.back().derivative = 0.0;
			i += sizeof(real);
			break;

		case Operation::PUTS:
			stack.resize(stack.size() + 1);
			stack.back().value = _last_strain;
			stack.back().derivative = 1.0;
			break;

		case Operation::ADD:
			(stack.end() - 2)->value += stack.back().value;
			(stack.end() - 2)->derivative += stack.back().derivative;
			stack.pop_back();
			break;

		case Operation::SUB:
			(stack.end() - 2)->value -= stack.back().value;
			(stack.end() - 2)->derivative -= stack.back().derivative; stack.pop_back();
			break;

		case Operation::MUL:
			(stack.end() - 2)->derivative =
				(stack.end() - 2)->value * stack.back().derivative + 
				(stack.end() - 2)->derivative * stack.back().value;
			(stack.end() - 2)->value *= stack.back().value;
			stack.pop_back();
			break;

		case Operation::DIV:
			(stack.end() - 2)->derivative =
				((stack.end() - 2)->derivative * stack.back().value -
				 (stack.end() - 2)->value * stack.back().derivative) / 
				 (stack.back().value * stack.back().value);
			(stack.end() - 2)->value /= stack.back().value;
			stack.pop_back();
			break;

		case Operation::SIN:
			stack.back().derivative = cos(stack.back().value) * stack.back().derivative;
			stack.back().value = sin(stack.back().value);
			break;

		case Operation::COS:
			stack.back().derivative = -sin(stack.back().value) * stack.back().derivative;
			stack.back().value = cos(stack.back().value);
			break;

		case Operation::LN:
			stack.back().derivative = stack.back().derivative / stack.back().value;
			stack.back().value = log(stack.back().value);
			break;

		case Operation::EXP:
			stack.back().derivative = stack.back().value * stack.back().derivative;
			stack.back().value = exp(stack.back().value);
			break;

		default:
			assert(false);
		}
	}

	//Checking stack
	if (stack.size() != 1)
		throw std::logic_error("NonlinearMaterial: stack size at end is not 1\n");

	//Saving result
	_last_stress = stack[0].value;
	_last_derivative = stack[0].derivative;
}