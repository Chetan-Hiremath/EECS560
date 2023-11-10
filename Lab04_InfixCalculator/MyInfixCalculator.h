#ifndef __MYINFIXCALCULATOR_H__
#define __MYINFIXCALCULATOR_H__

#include <algorithm>
#include <string>

#include "MyStack.h"
#include "MyVector.h"

class MyInfixCalculator{

  public:
    
    MyInfixCalculator()
    {

    }

    ~MyInfixCalculator()
    {
     
    }

    double calculate(const std::string& s)
    {
      MyVector<std::string> infix;
      MyVector<std::string> postfix;
      tokenize(s, infix);
      infixToPostfix(infix, postfix);
      double tempValue = calPostfix(postfix);
      return tempValue;
    }

  private:

    // returns operator precedance; the smaller the number the higher precedence
    // returns -1 if the operator is invalid
    // does not consider parenthesis
    int operatorPrec(const char c) const
    {
        switch(c)
        {
            case '*':
                return 2;
            case '/':
                return 2;
            case '+':
                return 3;
            case '-':
                return 3;
            default:
                return -1;
        }
    }

    // checks if a character corresponds to a valid parenthesis
    bool isValidParenthesis(const char c) const
    {
        switch(c)
        {
            case '(':
                return true;
            case ')':
                return true;
            default:
                return false;
        }
    }

    // checks if a character corresponds to a valid digit
    bool isDigit(const char c) const
    {
        if(c >= '0' && c <= '9')
            return true;
        return false;
    }

    // computes binary operation given the two operands and the operator in their string form
    double computeBinaryOperation(const std::string& ornd1, const std::string& ornd2, const std::string& opt) const
    {
        double o1 = std::stod(ornd1);
        double o2 = std::stod(ornd2);
        switch(opt[0])
        {
            case '+':
                return o1 + o2;
            case '-':
                return o1 - o2;
            case '*':
                return o1 * o2;
            case '/':
                return o1 / o2;
            default:
                std::cout << "Error: unrecognized operator: " << opt << std::endl;
                return 0.0;
        }
    }


    // tokenizes an infix string s into a set of tokens (operands or operators)
    void tokenize(const std::string& s, MyVector<std::string>& tokens)
    {
        size_t i = 0;
        bool Negative = true;
        while (i < s.length())
        {
          std::string temp = "";
          if ((s[i] == '-') && Negative)
          {
            temp += s[i++];
          }
          while (isDigit(s[i]) || (s[i] == '.'))
          {
            temp += s[i++];
          }
          Negative = false;
          if (temp == "") 
          {
            temp += s[i++];
            if (temp != ")") 
            {         
              Negative = true;
            }  
          }
          tokens.push_back(temp);
        }
    }

    // converts a set of infix tokens to a set of postfix tokens
    void infixToPostfix(MyVector<std::string>& infix_tokens, MyVector<std::string>& postfix_tokens)
    {
      MyStack<std::string> stack;
      for (int i = 0; i < infix_tokens.size(); ++i)
      {
        if (isDigit(infix_tokens[i][1]) || isDigit(infix_tokens[i][2]))
        {
          postfix_tokens.push_back(infix_tokens[i]);
        } 
        else
        {
          if (stack.empty())
          {
            stack.push(infix_tokens[i]);
          }
          else 
          {
            if (infix_tokens[i] == "(" || infix_tokens[i] == ")")
            {
              if (infix_tokens[i] == "(")
              {
                stack.push(infix_tokens[i]);
              }
              if (infix_tokens[i] == ")")
              {
                while (stack.top() != "(")
                {
                  postfix_tokens.push_back(stack.top());
                  stack.pop();
                }
                stack.pop();
              }
            }
            else 
            {
              if (stack.top() == "(" || stack.top() == ")")
              {
                stack.push(infix_tokens[i]);
              }
              else 
              {
                while ((operatorPrec(infix_tokens[i][0]) >= operatorPrec(stack.top()[0])) && (operatorPrec(stack.top()[0]) != -1) && (operatorPrec(infix_tokens[i][0]) != 1))
                {
                  postfix_tokens.push_back(stack.top());
                  stack.pop();
                  if (stack.empty())
                  {
                    break;
                  }
                }
                stack.push(infix_tokens[i]);
              }
            } 
          }
        } 
      }  
      while (!stack.empty())
      {
        postfix_tokens.push_back(stack.top());
        stack.pop();
      }  
    }

    // calculates the final result from postfix tokens
    double calPostfix(const MyVector<std::string>& postfix_tokens) const
    {
      MyStack <std::string> stack;
      for (int i = 0; i < postfix_tokens.size(); ++i)
      {
        if (postfix_tokens[i].length() > 1)
        {
          stack.push(postfix_tokens[i]);
        }
        else 
        {
          std::string a = stack.top();
          stack.pop();
          std::string b = stack.top();
          stack.pop();
          stack.push(std::to_string(computeBinaryOperation(b,a,postfix_tokens[i])));
        }
      }
      return (std::stod(stack.top()));
    }
};

#endif // __MYINFIXCALCULATOR_H__
