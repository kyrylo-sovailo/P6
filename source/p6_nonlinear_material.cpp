#include "../header/p6_nonlinear_material.h"
#include <stdexcept>
#include <cmath>
#include <cstring>

p6::NonlinearMaterial::NonlinearMaterial(const String &name, const String &formula)
{
	if (name == "") throw std::runtime_error("Name of material cannot be empty");
	_name = name;
	_formula = formula;
	_last_strain = nan("");

	//Prinary check (check illegal symbols)
	for (unsigned int i = 0; i < formula.size(); i++)
	{
		char c = formula[i];
		if (!(('0' <= c && c <= '9')
		|| ('a' <= c && c <= 'z')
		|| strchr("()+-*/. ", c) != nullptr))
			throw std::runtime_error("NonlinearMaterial::parse() : illegal symbol");
	}

	//Parsing and primary check (check s)
	enum class EnumWord : unsigned char
	{
		REAL,
		STRAIN,
		ADD,
		SUB,
		MUL,
		DIV,
		NEG,
		SIN,
		COS,
		LN,
		EXP,
		OPEN,
		CLOSE
	};
	struct Word
	{
		EnumWord enum_;
		real real_;
	};
	std::vector<Word> left;
	const char *p = &formula[0];
	while (p <= &formula.back())
	{
		if ('0' <= *p && *p <= '9')
		{
			char *end;
			Word newword;
			newword.enum_ = EnumWord::REAL;
			newword.real_ = strtod(p, &end);
			p = end;
			left.push_back(newword);
		}
		else if ('a' <= *p && *p <= 'z')
		{
			unsigned int length = 0;
			while ('a' <= p[length] && p[length] <= 'z') length++;
			Word newword;
			if (length == 1 && memcmp(p, "s", 1) == 0) newword.enum_ = EnumWord::STRAIN;
			else if (length == 3 && memcmp(p, "sin", 3) == 0) newword.enum_ = EnumWord::SIN;
			else if (length == 3 && memcmp(p, "cos", 3) == 0) newword.enum_ = EnumWord::COS;
			else if (length == 2 && memcmp(p, "ln", 2) == 0) newword.enum_ = EnumWord::LN;
			else if (length == 3 && memcmp(p, "exp", 3) == 0) newword.enum_ = EnumWord::EXP;
			else throw std::runtime_error("NonlinearMaterial::parse(): invalid identifier\n");
			p += length;
			left.push_back(newword);
		}
		else if (strchr("+-*/()", *p) != nullptr)
		{
			Word newword;
			switch (*p)
			{
				case '+': newword.enum_ = EnumWord::ADD; break;
				case '-': newword.enum_ = EnumWord::SUB; break;
				case '*': newword.enum_ = EnumWord::MUL; break;
				case '/': newword.enum_ = EnumWord::DIV; break;
				case '(': newword.enum_ = EnumWord::OPEN; break;
				default : newword.enum_ = EnumWord::CLOSE; break;
			}
			left.push_back(newword);
			p++;
		}
		else if (*p == ' ') p++;
		else throw std::runtime_error("NonlinearMaterial::parse(): illegal word begin\n");
	}

	//Secondary check (ckeck sequent operators and identifiers, setermining unar minus)
	for (unsigned int i = 0; i <= left.size(); i++)
	{
		//Need numbers
		if (i == 0 ||
			left[i - 1].enum_ == EnumWord::ADD ||
			left[i - 1].enum_ == EnumWord::SUB ||
			left[i - 1].enum_ == EnumWord::MUL ||
			left[i - 1].enum_ == EnumWord::DIV ||
			left[i - 1].enum_ == EnumWord::NEG ||
			left[i - 1].enum_ == EnumWord::OPEN)
		{
			if (i < left.size())
			{
				if (left[i].enum_ == EnumWord::SUB) left[i].enum_ = EnumWord::NEG;
				if (left[i].enum_ == EnumWord::NEG			||
					left[i].enum_ == EnumWord::SIN			||
					left[i].enum_ == EnumWord::COS			||
					left[i].enum_ == EnumWord::LN			||
					left[i].enum_ == EnumWord::EXP			||
					left[i].enum_ == EnumWord::REAL			||
					left[i].enum_ == EnumWord::STRAIN		||
					left[i].enum_ == EnumWord::OPEN) continue;
			}
			throw std::runtime_error("NonlinearMaterial::parse(): illegal operator usage\n");
		}
		//Need operators
		else if (
			left[i - 1].enum_ == EnumWord::REAL		||
			left[i - 1].enum_ == EnumWord::STRAIN	||
			left[i - 1].enum_ == EnumWord::CLOSE)
		{
			if (i == left.size() ||
				left[i].enum_ == EnumWord::ADD ||
				left[i].enum_ == EnumWord::SUB ||
				left[i].enum_ == EnumWord::MUL ||
				left[i].enum_ == EnumWord::DIV ||
				left[i].enum_ == EnumWord::CLOSE) continue;
			throw std::runtime_error("NonlinearMaterial::parse(): illegal number usage\n");
		}
		//Need opening bracket
		else
		{
			if (i < left.size() && left[i].enum_ == EnumWord::OPEN) continue;
			throw std::runtime_error("NonlinearMaterial::parse(): illegal function usage\n");
		}
	}

	//Third check (counting brackets)
	unsigned int bracketcount = 0;
	for (unsigned int i = 0; i < left.size(); i++)
	{
		if (left[i].enum_ == EnumWord::OPEN)
		{
			bracketcount++;
		}
		else if (left[i].enum_ == EnumWord::CLOSE)
		{
			if (bracketcount-- == 0) break;
		}
	}
	if (bracketcount != 0)
		throw std::runtime_error("NonlinearMaterial::parse(): illegal bracket usage\n");

	//Transforming to inverse polish notation (pulling right)
	std::vector<EnumWord> stack;
	while (true)
	{
		//Begin, throws everyting out of stack or quits
		if (left.empty())
		{
			if (stack.empty()) break;
			else
			{
				_opcodes.push_back(static_cast<OperationCode>(stack.back()));
				stack.pop_back();
			}
		}
		//Opening bracket, throws everyting out of stack or terminates closing bracket
		else if (left.back().enum_ == EnumWord::OPEN)
		{
			if (stack.back() == EnumWord::CLOSE)
			{
				left.pop_back();
				stack.pop_back();
			}
			else
			{
				_opcodes.push_back(static_cast<OperationCode>(stack.back()));
				stack.pop_back();
			}
		}
		//Closing bracket and */ operators, go to stack
		else if (left.back().enum_ == EnumWord::CLOSE	||
			left.back().enum_ == EnumWord::MUL			||
			left.back().enum_ == EnumWord::DIV)
		{
			stack.push_back(left.back().enum_);
			left.pop_back();
		}
		//+- operators, throw */ operators out of stack or go to stack
		else if (left.back().enum_ == EnumWord::ADD || left.back().enum_ == EnumWord::SUB)
		{
			if (!stack.empty() && (stack.back() == EnumWord::MUL || stack.back() == EnumWord::DIV))
			{
				_opcodes.push_back(static_cast<OperationCode>(stack.back()));
				stack.pop_back();
			}
			else
			{
				stack.push_back(left.back().enum_);
				left.pop_back();
			}
		}
		//Functions go straight right
		else if (left.back().enum_ == EnumWord::STRAIN	||
			left.back().enum_ == EnumWord::NEG			||
			left.back().enum_ == EnumWord::SIN			||
			left.back().enum_ == EnumWord::COS			|| 
			left.back().enum_ == EnumWord::LN			|| 
			left.back().enum_ == EnumWord::EXP)
		{
			_opcodes.push_back(static_cast<OperationCode>(left.back().enum_));
			left.pop_back();
		}
		//Real if kind of function too, goes right
		else
		{
			_opcodes.push_back(OperationCode::PUTR);
			_opcodes.resize(_opcodes.size()+sizeof(real));
			memcpy(&_opcodes[_opcodes.size()-sizeof(real)], &left.back().real_, sizeof(real));
			left.pop_back();
		}
	}
};

const p6::String &p6::NonlinearMaterial::formula()
{
	return _formula;
};

p6::Material::Type p6::NonlinearMaterial::type() const noexcept
{
	return p6::Material::Type::nonlinear;
};

p6::real p6::NonlinearMaterial::stress(real strain) const
{
	if (strain != _last_strain)
	{
		_last_strain = strain;
		_calculate();
	}
	return _last_stress;
};

p6::real p6::NonlinearMaterial::derivative(real strain) const
{
	if (strain != _last_strain)
	{
		_last_strain = strain;
		_calculate();
	}
	return _last_derivative;
};

void p6::NonlinearMaterial::_calculate() const
{
	//Create stack
	struct StackElement
	{
		real value;
		real derivative;
	};
	std::vector<StackElement> stack;

	//Execute
	unsigned int i = 0;
	while (i < _opcodes.size())
	{
		switch (_opcodes[i++])
		{
		case OperationCode::PUTR:
			stack.resize(stack.size() + 1);
			memcpy(&stack.back().value, &_opcodes[i], sizeof(real));
			stack.back().derivative = 0.0;
			i += sizeof(real);
			break;

		case OperationCode::PUTS:
			stack.resize(stack.size() + 1);
			stack.back().value = _last_strain;
			stack.back().derivative = 1.0;
			break;

		case OperationCode::ADD:
			(stack.end() - 2)->value += stack.back().value;
			(stack.end() - 2)->derivative += stack.back().derivative;
			stack.pop_back();
			break;

		case OperationCode::SUB:
			(stack.end() - 2)->value -= stack.back().value;
			(stack.end() - 2)->derivative -= stack.back().derivative; stack.pop_back();
			break;

		case OperationCode::MUL:
			(stack.end() - 2)->derivative =
				(stack.end() - 2)->value * stack.back().derivative + 
				(stack.end() - 2)->derivative * stack.back().value;
			(stack.end() - 2)->value *= stack.back().value;
			stack.pop_back();
			break;

		case OperationCode::DIV:
			(stack.end() - 2)->derivative =
				((stack.end() - 2)->derivative * stack.back().value -
				 (stack.end() - 2)->value * stack.back().derivative) / 
				 (stack.back().value * stack.back().value);
			(stack.end() - 2)->value /= stack.back().value;
			stack.pop_back();
			break;

		case OperationCode::SIN:
			stack.back().derivative = cos(stack.back().value) * stack.back().derivative;
			stack.back().value = sin(stack.back().value);
			break;

		case OperationCode::COS:
			stack.back().derivative = -sin(stack.back().value) * stack.back().derivative;
			stack.back().value = cos(stack.back().value);
			break;

		case OperationCode::LN:
			stack.back().derivative = stack.back().derivative / stack.back().value;
			stack.back().value = log(stack.back().value);
			break;

		case OperationCode::EXP:
			stack.back().derivative = stack.back().value * stack.back().derivative;
			stack.back().value = exp(stack.back().value);
			break;

		default:
			throw std::logic_error("NonlinearMaterial: illegal instruction\n");
		}
	}

	//Checking stack
	if (stack.size() != 1)
		throw std::logic_error("NonlinearMaterial: stack size at end is not 1\n");

	//Saving result
	_last_stress = stack[0].value;
	_last_derivative = stack[0].derivative;
};
