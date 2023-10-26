#include "Automaton.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cassert>
#include <cctype>
#include <functional>
#include <cassert>

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
      char symbol;
      int fromState = fromEntry.first;
      for (const auto &symbolEntry : fromEntry.second)
      {
        if (symbolEntry.first == Epsilon)
        {
          symbol = '0';
        }
        else
        {
          symbol = symbolEntry.first;
        }
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
      printf("\na une epsilon transition\n");
      return false;
    }

    // Vérifie si l'automate a exactement un état initial
    if (initialStates.size() != 1)
    {
      printf("\nn'a pas exactement un état initial\n");
      return false;
    }

    // Vérifie pour chaque état, si il y a bien 0 ou 1 transition par symbole
    for (int state : states)
    {
      for (char symbol : alphabet)
      {
        int transitionCount = 0;
        for (int toState : states)
        {
          if (hasTransition(state, symbol, toState))
          {
            transitionCount++;
          }
        }
        if (transitionCount > 1)
        {
          printf("\nn'est pas déterministe\n");
          return false;
        }
      }
    }

    printf("\nest déterministe\n");
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

        // Si aucune transition n'est trouvée pour une combinaison état-symbole,
        // l'automate n'est pas complet
        if (!transitionExists)
        {
          return false;
        }
      }
    }

    // Si des transitions existent pour chaque combinaison état-symbole,
    // l'automate est complet
    return true;
  }

  Automaton Automaton::createComplete(const Automaton &automaton)
  {
    assert(automaton.isValid());
    Automaton completeAutomaton = automaton; // Copier l'automate donné
    // remove initialStates

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
    assert(automaton.isValid());
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
    assert(automaton.isValid());
    Automaton mirroredAutomaton;

    // Copy states and alphabet from the original automaton
    mirroredAutomaton.states = automaton.states;
    mirroredAutomaton.alphabet = automaton.alphabet;

    // Swap initial and final states
    mirroredAutomaton.initialStates = automaton.finalStates;
    mirroredAutomaton.finalStates = automaton.initialStates;

    // Reverse all transitions
    for (const auto &fromStatePair : automaton.transitions)
    {
      int fromState = fromStatePair.first;
      for (const auto &symbolPair : fromStatePair.second)
      {
        char symbol = symbolPair.first;
        for (int toState : symbolPair.second)
        {
          mirroredAutomaton.addTransition(toState, symbol, fromState);
        }
      }
    }

    return mirroredAutomaton;
  }

std::set<int> Automaton::makeTransition(const std::set<int> &origin, char alpha) const
{
    std::set<int> destinations; // Ensemble pour stocker les états de destination

    // Parcourir chaque état dans l'ensemble d'origine
    for (int state : origin)
    {
        // Vérifier si l'état a des transitions pour le symbole donné
        auto it = transitions.find(state);
        if (it != transitions.end())
        {
            auto symbolIt = it->second.find(alpha);
            if (symbolIt != it->second.end())
            {
                // Ajouter tous les états de destination à l'ensemble de destinations
                destinations.insert(symbolIt->second.begin(), symbolIt->second.end());
            }
        }
    }

    return destinations; // Retourner l'ensemble des états de destination
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

  /**
   * TP n°3
   */

  bool Automaton::isLanguageEmpty() const
  {

    std::set<int> visited;

    // Fonction récursive pour la recherche en profondeur
    std::function<bool(int)> dfs = [&](int currentState) -> bool
    {
      // Si l'état a déjà été visité, retourner false
      if (visited.find(currentState) != visited.end())
      {
        return false;
      }

      // Marquer l'état comme visité
      visited.insert(currentState);

      // Si l'état est final, retourner true
      if (finalStates.find(currentState) != finalStates.end())
      {
        return true;
      }

      // Parcourir les transitions à partir de l'état actuel
      for (const auto &symbolPair : transitions.at(currentState))
      {
        for (int nextState : symbolPair.second)
        {
          if (dfs(nextState))
          {
            return true;
          }
        }
      }

      return false;
    };

    // Lancer la recherche en profondeur à partir de chaque état initial
    for (int initialState : initialStates)
    {
      if (dfs(initialState))
      {
        return false; // Le langage n'est pas vide
      }
    }

    return true; // Le langage est vide
  }

  void removeNonAccessibleStates()
  {
    // TODO
  }

  void removeNonCoAccessibleStates()
  {
    // TODO
  }

} // namespace fa