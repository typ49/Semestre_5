#include "Automaton.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cassert>
#include <cctype>
#include <functional>
#include <cassert>
#include <utility> // std::pair

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
    // vérifie si le symbole donné est valide et s'il n'existe pas déjà
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
      if (hasState(from) && hasState(to)) // les états doivent exister
      {
        if (hasSymbol(alpha) || alpha == Epsilon) // le symbole doit exister ou être un epsilon
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
    if (hasState(from) && hasState(to) && (hasSymbol(alpha) || alpha == Epsilon) && hasTransition(from, alpha, to))
    {
      transitions[from][alpha].erase(to);

      // Supprime l'entrée [from][alpha] si l'ensemble est vide
      if (transitions[from][alpha].empty())
      {
        transitions[from].erase(alpha);

        // Optionnel: Supprimer également la clé 'from' si elle ne contient plus de transitions
        if (transitions[from].empty())
        {
          transitions.erase(from);
        }
      }

      return true;
    }
    return false;
  }

  bool Automaton::hasTransition(int from, char alpha, int to) const
  {
    if (hasState(from) && hasState(to)) // les états doivent exister
    {
      if (hasSymbol(alpha) || alpha == Epsilon) // le symbole doit exister ou être un epsilon
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

  void Automaton::dotPrint(std::ostream &os) const
  {
    os << "digraph Automaton {" << std::endl;
    os << "  rankdir=LR;" << std::endl;
    os << "  size=\"8,5\"" << std::endl;
    os << "  node [shape = doublecircle];";

    // Marquer les états finaux avec un double cercle
    for (int finalState : finalStates)
    {
      os << " " << finalState;
    }

    os << ";" << std::endl;
    os << "  node [shape = circle];" << std::endl;

    // Marquer les états initiaux avec une flèche venant d'un point invisible
    for (int initialState : initialStates)
    {
      os << "  init" << initialState << " [shape=point];" << std::endl;
      os << "  init" << initialState << " -> " << initialState << ";" << std::endl;
    }

    // Afficher les transitions
    for (const auto &fromState : transitions)
    {
      for (const auto &bySymbol : fromState.second)
      {
        for (int toState : bySymbol.second)
        {
          os << "  " << fromState.first << " -> " << toState;
          os << " [ label = \"" << bySymbol.first << "\" ];" << std::endl;
        }
      }
    }

    os << "}" << std::endl;
  }

  /**
   * TP n°2
   */

  bool Automaton::hasEpsilonTransition() const
  {
    assert(isValid());
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
    assert(isValid());

    // Vérifie si l'automate a des transitions epsilon
    if (hasEpsilonTransition())
    {
      // printf("\na une epsilon transition\n");
      return false;
    }

    // Vérifie si l'automate a exactement un état initial
    if (initialStates.size() != 1)
    {
      // printf("\nn'a pas exactement un état initial\n");
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
          // printf("\nn'est pas déterministe\n");
          return false;
        }
      }
    }

    // printf("\nest déterministe\n");
    return true;
  }

  bool Automaton::isComplete() const
  {
    assert(isValid());
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

    int trapState = -1;          // Utiliser -1 comme état poubelle
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
    assert(isValid());
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
    assert(isValid());
    std::set<int> currentStates = initialStates; // Ensemble pour stocker les états actuels
    std::set<int> nextStates;                    // Ensemble pour stocker les états suivants

    // Parcourir chaque symbole dans le mot
    for (char alpha : word)
    {
      // Utiliser makeTransition pour obtenir l'ensemble d'états suivants
      nextStates = makeTransition(currentStates, alpha);

      // Mettre à jour l'ensemble d'états actuels pour le prochain tour de la boucle
      currentStates = nextStates;

      // Vous pouvez vider nextStates si vous le souhaitez, mais ce n'est pas strictement nécessaire
      nextStates.clear();
    }

    return currentStates; // Retourner l'ensemble des états atteints après avoir lu le mot
  }

  bool Automaton::match(const std::string &word) const
  {
    assert(isValid());
    // Utiliser readString pour obtenir l'ensemble d'états atteints après avoir lu le mot
    std::set<int> finalStatesReached = readString(word);

    // Vérifier si l'ensemble d'états atteints contient au moins un état final
    for (int state : finalStatesReached)
    {
      if (finalStates.find(state) != finalStates.end())
      {
        return true; // Le mot est accepté par l'automate
      }
    }

    return false; // Le mot n'est pas accepté par l'automate
  }

  /**
   * TP n°3
   */

  bool Automaton::isLanguageEmpty() const {
    assert(isValid());

    std::set<int> visited;

    // Fonction récursive pour la recherche en profondeur (DFS)
    std::function<bool(int)> dfs = [&](int currentState) -> bool {
        // Si l'état a déjà été visité, retourner false
        if (visited.find(currentState) != visited.end()) {
            return false;
        }

        // Marquer l'état comme visité
        visited.insert(currentState);

        // Si l'état est final, retourner true
        if (finalStates.find(currentState) != finalStates.end()) {
            return true;
        }

        // Vérifier l'existence de transitions pour l'état actuel avant de parcourir
        if (transitions.find(currentState) != transitions.end()) {
            // Parcourir les transitions à partir de l'état actuel
            for (const auto &symbolPair : transitions.at(currentState)) {
                for (int nextState : symbolPair.second) {
                    if (dfs(nextState)) {
                        return true;
                    }
                }
            }
        }

        return false;
    };

    // Lancer la recherche en profondeur à partir de chaque état initial
    for (int initialState : initialStates) {
        if (dfs(initialState)) {
            return false; // Le langage n'est pas vide
        }
    }

    return true; // Le langage est vide
}

  void Automaton::removeNonAccessibleStates()
  {
    assert(isValid());
    std::set<int> accessibleStates; // Ensemble des états accessibles
    std::set<int> toExplore;        // Ensemble des états à explorer
    std::set<int> toRemove;         // Ensemble des états à retirer

    // Ajouter tous les états initiaux à l'ensemble des états à explorer
    for (int initialState : initialStates)
    {
      toExplore.insert(initialState);
      accessibleStates.insert(initialState);
    }

    // Tant qu'il y a des états à explorer
    while (!toExplore.empty())
    {
      // Prendre un état à explorer
      auto it = toExplore.begin();
      int currentState = *it;
      toExplore.erase(it);

      // Trouver tous les états accessibles depuis cet état
      for (const auto &symbol : alphabet)
      {
        std::set<int> nextStates = makeTransition({currentState}, symbol);

        for (int nextState : nextStates)
        {
          // Si l'état n'a pas déjà été marqué comme accessible
          if (accessibleStates.find(nextState) == accessibleStates.end())
          {
            accessibleStates.insert(nextState);
            toExplore.insert(nextState);
          }
        }
      }
    }

    // Identifier les états non accessibles
    for (int state : states)
    {
      if (accessibleStates.find(state) == accessibleStates.end())
      {
        toRemove.insert(state);
      }
    }

    // Supprimer les états non accessibles
    for (int state : toRemove)
    {
      removeState(state);
      states.erase(state);
    }
  }

  void Automaton::removeNonCoAccessibleStates()
  {
    assert(isValid());
    std::set<int> coAccessibleStates; // Ensemble des états co-accessibles
    std::set<int> toExplore;          // Ensemble des états à explorer
    std::set<int> toRemove;           // Ensemble des états à supprimer

    // Ajouter tous les états finaux à l'ensemble des états à explorer
    for (int finalState : finalStates)
    {
      if (states.find(finalState) != states.end())
      {
        toExplore.insert(finalState);
        coAccessibleStates.insert(finalState);
      }
    }

    // Trouver les états co-accessibles
    while (!toExplore.empty())
    {
      auto it = toExplore.begin();
      int currentState = *it;
      toExplore.erase(it);

      for (const auto &pair : transitions)
      {
        if (states.find(pair.first) == states.end())
          continue;

        for (const auto &innerPair : pair.second)
        {
          if (innerPair.second.find(currentState) != innerPair.second.end())
          {
            int previousState = pair.first;

            if (coAccessibleStates.find(previousState) == coAccessibleStates.end())
            {
              coAccessibleStates.insert(previousState);
              toExplore.insert(previousState);
            }
          }
        }
      }
    }

    // Identifier les états à supprimer
    for (const auto &state : states)
    {
      if (coAccessibleStates.find(state) == coAccessibleStates.end())
      {
        toRemove.insert(state);
      }
    }

    // Supprimer les états non co-accessibles
    for (const auto &state : toRemove)
    {
      removeState(state);
      states.erase(state);
    }
  }

  /**
   * TP n°4
   */

  Automaton Automaton::createIntersection(const Automaton &lhs, const Automaton &rhs) {
    assert(lhs.isValid());
    assert(rhs.isValid());

    Automaton intersectionAutomaton;
    std::map<std::pair<int, int>, int> statePairsMap;
    int newStateIndex = 0;

    // vérification langage disjoint
    bool disjoint = true;
    for (char lhsSymbol : lhs.alphabet) {
        for (char rhsSymbol : rhs.alphabet) {
            // vérifie si les deux symboles ne sont pas égaux
            if (lhsSymbol == rhsSymbol) {
                disjoint = false;
            }
        }
    }

    // Unifier les alphabets des deux automates
    intersectionAutomaton.alphabet = lhs.alphabet;
    for (auto symbol : rhs.alphabet) {
        intersectionAutomaton.addSymbol(symbol);
    }

    // Parcourir chaque paire d'états initiaux de lhs et rhs
    for (auto lhsInitState : lhs.initialStates) {
        for (auto rhsInitState : rhs.initialStates) {
            int combinedInitialState = newStateIndex++;
            intersectionAutomaton.addState(combinedInitialState);
            intersectionAutomaton.setStateInitial(combinedInitialState);
            statePairsMap[std::make_pair(lhsInitState, rhsInitState)] = combinedInitialState;
        }
    }

    // Parcourir toutes les combinaisons d'états de lhs et rhs
    for (auto lhsState : lhs.states) {
        for (auto rhsState : rhs.states) {
            auto statePair = std::make_pair(lhsState, rhsState);

            if (statePairsMap.find(statePair) == statePairsMap.end()) {
                int combinedState = newStateIndex++;
                intersectionAutomaton.addState(combinedState);
                statePairsMap[statePair] = combinedState;
            }
            int combinedState = statePairsMap[statePair];

            // Marquer comme état final uniquement si les deux états sont finaux
            if (lhs.finalStates.find(lhsState) != lhs.finalStates.end() && rhs.finalStates.find(rhsState) != rhs.finalStates.end() && disjoint == false) {
                intersectionAutomaton.setStateFinal(combinedState);
            }

            // Créer des transitions pour les symboles communs
            for (char symbol : intersectionAutomaton.alphabet) {
                if (!lhs.hasSymbol(symbol) || !rhs.hasSymbol(symbol)) {
                    continue; // Ignorer les symboles non communs
                }

                std::set<int> nextLhsStates = lhs.makeTransition({lhsState}, symbol);
                std::set<int> nextRhsStates = rhs.makeTransition({rhsState}, symbol);

                for (int nextLhsState : nextLhsStates) {
                    for (int nextRhsState : nextRhsStates) {
                        auto nextStatePair = std::make_pair(nextLhsState, nextRhsState);
                        if (statePairsMap.find(nextStatePair) == statePairsMap.end()) {
                            int nextState = newStateIndex++;
                            intersectionAutomaton.addState(nextState);
                            statePairsMap[nextStatePair] = nextState;
                        }
                        int nextState = statePairsMap[nextStatePair];

                        intersectionAutomaton.addTransition(combinedState, symbol, nextState);
                    }
                }
            }
        }
    }

    return intersectionAutomaton;
}


  bool Automaton::hasEmptyIntersectionWith(const Automaton &other) const
  {
    // TODO
    return false;
  }

  /**
   * TP n°5
   */

  bool Automaton::isIncludedIn(const Automaton &other) const
  {
    // TODO
    return false;
  }

  Automaton Automaton::createDeterministic(const Automaton &other)
  {
    // TODO
    Automaton deterministic;
    return deterministic;
  }

  /**
   * TP n°6
   */

  Automaton Automaton::createMinimalBrzozowski(const Automaton &other)
  {
    // TODO
    Automaton minimalBrzozowski;
    return minimalBrzozowski;
  }

} // namespace fa