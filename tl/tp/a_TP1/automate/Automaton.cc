#include "Automaton.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cassert>
#include <cctype>

namespace fa
{

  // cette structure permet de stocker les transitions, les etats fi  finaux et initiaux et l'alphabet de l'automate
  Automaton::Automaton()
  {
  }

  bool Automaton::isValid() const
  {
    return !alphabet.empty() && !states.empty();
  }

  bool Automaton::addSymbol(char symbol)
  {
    if (symbol != Epsilon && !hasSymbol(symbol))
    {
      alphabet.insert(symbol);
      return true;
    }
    return false;
  }

  bool Automaton::removeSymbol(char symbol)
  {
    if (hasSymbol(symbol))
    {
      alphabet.erase(symbol);
      return true;
    }
    return false;
  }

  bool Automaton::hasSymbol(char symbol) const
  {
    return alphabet.find(symbol) != alphabet.end();
  }

  std::size_t Automaton::countSymbols() const
  {
    return alphabet.size();
  }

  bool Automaton::addState(int state)
  {
    if (states.find(state) == states.end())
    {
      states.insert(state);
      return true;
    }
    return false;
  }

  bool Automaton::removeState(int state)
  {
    if (states.find(state) != states.end())
    {
      states.erase(state);
      return true;
    }
    return false;
  }

  bool Automaton::hasState(int state) const
  {
    return states.find(state) != states.end();
  }

  std::size_t Automaton::countStates() const
  {
    return states.size();
  }

  void Automaton::setStateInitial(int state)
  {
    if (hasState(state))
    {
      initialStates.insert(state);
    }
    return;
  }

  bool Automaton::isStateInitial(int state) const
  {
    return initialStates.find(state) != initialStates.end();
  }

  void Automaton::setStateFinal(int state)
  {
    if (hasState(state))
    {
      finalStates.insert(state);
    }
    return;
  }

  bool Automaton::isStateFinal(int state) const
  {
    return finalStates.find(state) != finalStates.end();
  }

  bool Automaton::addTransition(int from, char alpha, int to)
  {
    if (hasState(from) && hasState(to) && hasSymbol(alpha))
    {
      transitions[from][alpha].insert(to);
      return true;
    }
    return false;
  }

  bool Automaton::removeTransition(int from, char alpha, int to)
  {
    if (hasState(from) && hasState(to) && hasSymbol(alpha))
    {
      transitions[from][alpha].erase(to);
      return true;
    }
    return false;
  }

  bool Automaton::hasTransition(int from, char alpha, int to) const
  {
    if (hasState(from) && hasState(to) && hasSymbol(alpha))
    {
      return transitions.find(from) != transitions.end() && transitions.at(from).find(alpha) != transitions.at(from).end() && transitions.at(from).at(alpha).find(to) != transitions.at(from).at(alpha).end();
    }
    return false;
  }

  std::size_t Automaton::countTransitions() const
  {
    std::size_t count = 0;
    for (auto from : transitions)
    {
      for (auto alpha : from.second)
      {
        count += alpha.second.size();
      }
    }
    return count;
  }
}
