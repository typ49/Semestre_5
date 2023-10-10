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

  /**
   * TP n°1
   */
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
    if (!hasState(state))
    {
      states.insert(state);
      return true;
    }
    return false;
  }

  bool Automaton::removeState(int state)
  {
    if (hasState(state))
    {
      // supprime l'état
      states.erase(state);
      initialStates.erase(state);
      finalStates.erase(state);
      // supprime les transition associées à l'état supprimé
      for (auto from : transitions)
      {
        for (auto alpha : from.second)
        {
          alpha.second.erase(state);
        }
      }
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
    // Vérifie si l'état de départ, l'état d'arrivée et le symbole sont valides et si la transition n'existe pas déjà
    // peux ajouter une epsilon transition
    if (!hasTransition(from, alpha, to))
    {
      if (hasState(from) && hasState(to))
      {
        if (hasSymbol(alpha) || alpha == Epsilon)
        {
          transitions[from][alpha].insert(to);
          return true;
        }
      }
    }
    return false;
  }

  bool Automaton::removeTransition(int from, char alpha, int to)
  {
    if (hasState(from) && hasState(to) && hasSymbol(alpha) && hasTransition(from, alpha, to))
    {
      transitions[from][alpha].erase(to);
      return true;
    }
    return false;
  }

  bool Automaton::hasTransition(int from, char alpha, int to) const
  {
    if (hasState(from) && hasState(to))
    {
      if (hasSymbol(alpha) || alpha == Epsilon)
      {
        return transitions.find(from) != transitions.end() && transitions.at(from).find(alpha) != transitions.at(from).end() && transitions.at(from).at(alpha).find(to) != transitions.at(from).at(alpha).end();
      }
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

  void Automaton::prettyPrint(std::ostream &os) const
  {
    os << std::endl
       << "Automaton:" << std::endl;

    // Afficher l'alphabet
    os << "Alphabet: { ";
    for (char symbol : alphabet)
    {
      os << symbol << " ";
    }
    os << "}" << std::endl;

    // Afficher les états
    os << "States: { ";
    for (int state : states)
    {
      os << state << " ";
    }
    os << "}" << std::endl;

    // Afficher les états initiaux
    os << "Initial States: { ";
    for (int initState : initialStates)
    {
      os << initState << " ";
    }
    os << "}" << std::endl;

    // Afficher les états finaux
    os << "Final States: { ";
    for (int finalState : finalStates)
    {
      os << finalState << " ";
    }
    os << "}" << std::endl;

    // Afficher les transitions
    os << "Transitions:" << std::endl;
    for (const auto &fromEntry : transitions)
    {
      int fromState = fromEntry.first;
      for (const auto &symbolEntry : fromEntry.second)
      {
        char symbol = symbolEntry.first;
        for (int toState : symbolEntry.second)
        {
          os << "  " << fromState << " --" << symbol << "-> " << toState << std::endl;
        }
      }
    }
    os << std::endl;
  }

  /**
   * TP n°2
   */

  bool Automaton::hasEpsilonTransition() const
  {
    for (auto from : transitions)
    {
      for (auto alpha : from.second)
      {
        if (alpha.first == Epsilon)
        {
          return true;
        }
      }
    }
    return false;
  }

  bool Automaton::isDeterministic() const
  {
    // Vérifie si l'automate a des transitions epsilon
    if (hasEpsilonTransition())
    {
      printf("a une epsilon transition");
      return false;
    }

    // Vérifie si l'automate a exactement un état initial
    if (initialStates.size() != 1)
    {
      printf("n'a pas exactement un état initial");
      return false;
    }

    // Vérifie si chaque état a une transition définie pour chaque symbole de l'alphabet
    for (auto state : states)
    {
      for (auto alpha : alphabet)
      {
        int transitionCount = 0;

        // Compte le nombre de transitions pour l'état courant et le symbole courant
        if (transitions.find(state) != transitions.end() && transitions.at(state).find(alpha) != transitions.at(state).end())
        {
          transitionCount = transitions.at(state).at(alpha).size();
        }

        // Si aucune transition ou plus d'une transition est définie pour une combinaison état-symbole, l'automate n'est pas déterministe
        if (transitionCount != 1)
        {
          printf("n'est pas déterministe");
          return false;
        }
      }
    }
  printf("est déterministe");
    return true;
  }

  bool Automaton::isComplete() const
  {
    // Parcourir chaque état de l'automate
    for (int state : states)
    {
      // Parcourir chaque symbole de l'alphabet
      for (char symbol : alphabet)
      {
        bool transitionExists = false;

        // Vérifier s'il existe une transition pour cet état et ce symbole
        for (int toState : states)
        {
          if (hasTransition(state, symbol, toState))
          {
            transitionExists = true;
            break;
          }
        }

        if (!transitionExists)
        {
          return false; // Si aucune transition n'est trouvée pour une combinaison état-symbole, l'automate n'est pas complet
        }
      }
    }
    return true; // Si des transitions existent pour chaque combinaison état-symbole, l'automate est complet
  }

  Automaton Automaton::createComplete(const Automaton &automaton)
  {
    Automaton completeAutomaton = automaton; // Copier l'automate donné

    int trapState = -1;          // Utiliser -1 comme état poubelle, mais vous pouvez choisir une autre valeur si nécessaire
    bool trapStateAdded = false; // Indicateur pour savoir si l'état poubelle a été ajouté

    // Parcourir chaque état de l'automate
    for (int state : automaton.states)
    {
      // Parcourir chaque symbole de l'alphabet
      for (char symbol : automaton.alphabet)
      {
        // Vérifier s'il existe une transition pour cet état et ce symbole
        bool transitionExists = false;
        for (int toState : automaton.states)
        {
          if (automaton.hasTransition(state, symbol, toState))
          {
            transitionExists = true;
            break;
          }
        }

        // Si aucune transition n'existe pour cette combinaison état-symbole
        if (!transitionExists)
        {
          // Ajouter l'état poubelle si ce n'est pas déjà fait
          if (!trapStateAdded)
          {
            completeAutomaton.addState(trapState);
            trapStateAdded = true;
          }

          // Ajouter une transition vers l'état poubelle
          completeAutomaton.addTransition(state, symbol, trapState);
        }
      }
    }

    // Si l'état poubelle a été ajouté, ajouter des transitions de l'état poubelle vers lui-même pour chaque symbole
    if (trapStateAdded)
    {
      for (char symbol : automaton.alphabet)
      {
        completeAutomaton.addTransition(trapState, symbol, trapState);
      }
    }

    return completeAutomaton;
  }

  Automaton Automaton::createComplement(const Automaton &automaton)
  {
    Automaton complement;
    complement.alphabet = automaton.alphabet;
    complement.states = automaton.states;
    complement.initialStates = automaton.initialStates;
    for (auto state : automaton.states)
    {
      if (!automaton.isStateFinal(state))
      {
        complement.setStateFinal(state);
      }
    }
    return complement;
  }

  Automaton Automaton::createMirror(const Automaton &automaton)
  {
    // TODO
    return Automaton(); // Ajouté temporairement pour éviter les erreurs de compilation
  }

  std::set<int> Automaton::makeTransition(const std::set<int> &origin, char alpha) const
  {
    // TODO
    return std::set<int>(); // Ajouté temporairement pour éviter les erreurs de compilation
  }

  std::set<int> Automaton::readString(const std::string &word) const
  {
    // TODO
    return std::set<int>(); // Ajouté temporairement pour éviter les erreurs de compilation
  }

  bool Automaton::match(const std::string &word) const
  {
    // TODO
    return false; // Ajouté temporairement pour éviter les erreurs de compilation
  }

} // namespace fa