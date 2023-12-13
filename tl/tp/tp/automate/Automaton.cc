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
    if (isgraph(symbol) && !hasSymbol(symbol))
    {
      alphabet.insert(symbol);
      return true;
    }
    return false;
  }

  bool Automaton::removeSymbol(char symbol)
  {
    // Vérifier si le symbole est dans l'alphabet
    if (alphabet.find(symbol) == alphabet.end())
    {
      // Le symbole n'est pas dans l'alphabet, renvoyer false
      return false;
    }

    // Le symbole est dans l'alphabet, procéder à la suppression
    alphabet.erase(symbol);

    // Supprimer les transitions associées à ce symbole
    for (auto &stateTransitions : transitions)
    {
      // Supprimer l'entrée correspondante au symbole dans chaque map
      stateTransitions.second.erase(symbol);
    }

    // Le symbole a été effectivement supprimé, renvoyer true
    return true;
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
    if (!hasState(state) && state >= 0)
    {
      states.insert(state);
      return true;
    }
    return false;
  }

  bool Automaton::removeState(int state)
  {
    if (!hasState(state))
    {
      return false;
    }

    // Supprimer l'état des différents ensembles
    states.erase(state);
    initialStates.erase(state);
    finalStates.erase(state);

    // Supprimer toutes les transitions sortantes de cet état
    transitions.erase(state);

    // Supprimer toutes les transitions entrantes vers cet état
    for (auto &from : transitions)
    {
      for (auto &alpha : from.second)
      {
        alpha.second.erase(state);
      }
    }

    // Supprimer les entrées vides dans la map de transitions
    for (auto it = transitions.begin(); it != transitions.end();)
    {
      if (it->second.empty())
      {
        it = transitions.erase(it);
      }
      else
      {
        ++it;
      }
    }

    return true;
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
    // int count = transitions.size();

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
            // printf("state n°%d, symbol %c, toState n°%d\n", state, symbol, toState);
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

    int trapState = 0;
    while (completeAutomaton.hasState(trapState))
    {
      trapState++;
    }
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

    // Déterminisation si nécessaire
    Automaton detAutomaton = automaton.isDeterministic() ? automaton : Automaton::createDeterministic(automaton);

    // Rendre l'automate complet si nécessaire
    Automaton completeAutomaton = detAutomaton.isComplete() ? detAutomaton : Automaton::createComplete(detAutomaton);

    // Création de l'automate complémentaire
    Automaton complement;
    complement.alphabet = completeAutomaton.alphabet;
    complement.states = completeAutomaton.states;
    complement.transitions = completeAutomaton.transitions;
    complement.initialStates = completeAutomaton.initialStates;

    // Inverser les états finaux et non finaux
    for (auto state : completeAutomaton.states)
    {
      if (!completeAutomaton.isStateFinal(state))
        complement.setStateFinal(state);
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

  std::set<int> Automaton::makeTransition_reverse(const std::set<int> &destination, char alpha) const
  {
    assert(isValid());

    std::set<int> origins; // Ensemble pour stocker les états d'origine

    // Parcourir chaque état
    for (int state : states)
    {
      // Vérifier si l'état a des transitions pour le symbole donné
      auto it = transitions.find(state);
      if (it != transitions.end())
      {
        auto symbolIt = it->second.find(alpha);
        if (symbolIt != it->second.end())
        {
          // Vérifier si l'état de destination de la transition est dans l'ensemble de destination
          for (int dest : symbolIt->second)
          {
            if (destination.find(dest) != destination.end())
            {
              // Ajouter l'état d'origine à l'ensemble des états d'origine
              origins.insert(state);
              break;
            }
          }
        }
      }
    }

    return origins;
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

  bool Automaton::isLanguageEmpty() const
  {
    assert(isValid());

    std::set<int> visited;

    // Fonction récursive pour la recherche en profondeur (DFS)
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

      // Vérifier l'existence de transitions pour l'état actuel avant de parcourir
      if (transitions.find(currentState) != transitions.end())
      {
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

  void Automaton::removeNonAccessibleStates()
  {
    assert(isValid());
    std::set<int> accessibleStates; // Ensemble des états accessibles
    std::set<int> toExplore;        // Ensemble des états à explorer
    std::set<int> toRemove;         // Ensemble des états à retirer

    // Ajouter tous les états initiaux à l'ensemble des états à explorer et à l'ensemble des états accessibles
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

    if (!isValid())
    {
      addState(0);
      addSymbol('a');
    }
  }

  void Automaton::removeNonCoAccessibleStates()
  {
    assert(isValid());
    std::set<int> coAccessibleStates; // Ensemble des états co-accessibles
    std::set<int> toExplore;          // Ensemble des états à explorer
    std::set<int> toRemove;           // Ensemble des états à supprimer

    // Ajouter tous les états finaux à l'ensemble des états à explorer et à l'ensemble des états co-accessibles
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

      // Trouver tous les états co-accessibles vers cet état
      for (auto &symbol : alphabet)
      {
        std::set<int> nextStates = makeTransition_reverse({currentState}, symbol);

        for (int nextState : nextStates)
        {
          // Si l'état n'a pas déjà été marqué comme co-accessible
          if (coAccessibleStates.find(nextState) == coAccessibleStates.end())
          {
            coAccessibleStates.insert(nextState);
            toExplore.insert(nextState);
          }
        }
      }
    }

    // Identifier les états non co-accessibles
    for (int state : states)
    {
      if (coAccessibleStates.find(state) == coAccessibleStates.end())
      {
        toRemove.insert(state);
        printf("state to remove %d\n", state);
      }
    }

    for (int state : toRemove)
    {
      removeState(state);
      states.erase(state);
    }

    if (!isValid())
    {
      addState(0);
      addSymbol('a');
    }
  }

  /**
   * TP n°4
   */

  Automaton Automaton::createIntersection(const Automaton &lhs, const Automaton &rhs)
  {
    assert(lhs.isValid());
    assert(rhs.isValid());

    bool disjoint = true;

    // vérifie si le language est disjoint

    for (auto lhsSymbol : lhs.alphabet)
    {
      for (auto rhsSymbol : rhs.alphabet)
      {
        if (lhsSymbol == rhsSymbol)
        {
          disjoint = false;
        }
      }
    }

    if (disjoint)
    {
      Automaton intersectionAutomaton;
      intersectionAutomaton.addSymbol('a');
      intersectionAutomaton.addState(0);
      return intersectionAutomaton;
    }

    Automaton intersectionAutomaton;
    std::map<std::pair<int, int>, int> productStates;
    int newState = 0;

    // crée l'alphabet de l'automate intersection
    for (char symbol : lhs.alphabet)
    {
      intersectionAutomaton.addSymbol(symbol);
    }
    for (char symbol : rhs.alphabet)
    {
      intersectionAutomaton.addSymbol(symbol);
    }

    // crée le produit cartésien des états
    for (int lhsState : lhs.states)
    {
      for (int rhsState : rhs.states)
      {
        productStates[std::make_pair(lhsState, rhsState)] = newState;
        intersectionAutomaton.addState(newState);
        newState++;
      }
    }

    // crée les états initiaux
    for (int lhsState : lhs.initialStates)
    {
      for (int rhsState : rhs.initialStates)
      {
        intersectionAutomaton.setStateInitial(productStates[std::make_pair(lhsState, rhsState)]);
      }
    }

    // crée les états finaux
    if (!disjoint)
    {
      for (int lhsState : lhs.finalStates)
      {
        for (int rhsState : rhs.finalStates)
        {
          intersectionAutomaton.setStateFinal(productStates[std::make_pair(lhsState, rhsState)]);
        }
      }
    }

    // crée les transitions
    for (const auto &lhsStatePair : lhs.transitions)
    { // lhsStatePair = (état, (symbole, ensemble d'états))
      for (const auto &lhsSymbolPair : lhsStatePair.second)
      { // lhsSymbolPair = (symbole, ensemble d'états)
        for (const auto &rhsStatePair : rhs.transitions)
        { // rhsStatePair = (état, (symbole, ensemble d'états))
          for (const auto &rhsSymbolPair : rhsStatePair.second)
          { // rhsSymbolPair = (symbole, ensemble d'états)
            if (lhsSymbolPair.first == rhsSymbolPair.first)
            {
              for (int lhsToState : lhsSymbolPair.second)
              {
                for (int rhsToState : rhsSymbolPair.second)
                {
                  intersectionAutomaton.addTransition(
                      productStates[std::make_pair(lhsStatePair.first, rhsStatePair.first)],
                      lhsSymbolPair.first,
                      productStates[std::make_pair(lhsToState, rhsToState)]);
                }
              }
            }
          }
        }
      }
    }

    return intersectionAutomaton;
  }

  bool Automaton::hasEmptyIntersectionWith(const Automaton &other) const
  {
    Automaton intersection = createIntersection(*this, other);
    return intersection.isLanguageEmpty();
  }

  /**
   * TP n°5
   */

  bool Automaton::isIncludedIn(const Automaton &other) const
  {
    // vérifie si les automates sont valides
    assert(isValid());
    assert(other.isValid());

    if (!isDeterministic())
    {
      printf("A n'est pas déterministe\n");
      Automaton deterministic = createDeterministic(*this); // rend l'automate A déterministe
      return deterministic.isIncludedIn(other);             // retourne le résultat avec ce nouvel automate
    }
    if (!other.isDeterministic())
    {
      printf("B n'est pas déterministe\n");
      Automaton deterministic = createDeterministic(other); // rend l'automate other déterministe
      return isIncludedIn(deterministic);                   // retourne le résultat avec ce nouvel automate
    }
    Automaton complementOther = createComplement(other); // fait un complement de l'automate other
    printf("complement\n");
    complementOther.prettyPrint(std::cout);
    Automaton intersection = createIntersection(*this, complementOther); // fait l'intersection des deux automates
    printf("intersection\n");
    intersection.prettyPrint(std::cout);
    printf("intersection\n empty ? %d\n", intersection.isLanguageEmpty());
    if (intersection.isLanguageEmpty())
    {
      return true; // si l'automate d'intersection est vide, ça veut dire qu'aucun mots de A n'est présent dans le mirroir de B et donc A est inclue dans B
    }

    return false;
  }

  Automaton Automaton::createDeterministic(const Automaton &other)
  {
    if (other.isDeterministic())
    {
      return other;
    }

    Automaton deterministic;
    deterministic.alphabet = other.alphabet;

    // Mappage d'ensembles d'états à des états dans l'automate déterministe
    std::map<std::set<int>, int> stateMapping;

    // File d'attente pour les ensembles d'états à traiter
    std::queue<std::set<int>> waitList;

    // Initialiser avec l'ensemble des états initiaux
    std::set<int> initialStates = other.initialStates;
    waitList.push(initialStates);
    deterministic.addState(0);        // Ajouter l'état initial
    deterministic.setStateInitial(0); // Définir l'état initial
    stateMapping[initialStates] = 0;  // Ajouter l'ensemble d'états initiaux à la table de mappage

    while (!waitList.empty())
    {
      std::set<int> currentStates = waitList.front();
      waitList.pop();

      // Vérifier si l'ensemble d'états actuel est final
      for (int state : currentStates)
      {
        if (other.isStateFinal(state))
        {
          deterministic.setStateFinal(stateMapping[currentStates]);
          break;
        }
      }

      // Parcourir chaque symbole de l'alphabet
      for (char symbol : deterministic.alphabet)
      {
        std::set<int> nextStates;

        // Parcourir chaque état de l'ensemble d'états actuel
        for (int state : currentStates)
        {
          // Ajouter les états de destination pour le symbole actuel
          std::set<int> states = other.makeTransition({state}, symbol);
          nextStates.insert(states.begin(), states.end());
        }

        // Si l'ensemble d'états de destination n'est pas vide
        if (!nextStates.empty())
        {
          // Si l'ensemble d'états de destination n'a pas encore été traité
          if (stateMapping.find(nextStates) == stateMapping.end())
          {
            // Ajouter l'ensemble d'états de destination à la file d'attente
            waitList.push(nextStates);

            // Ajouter un nouvel état à l'automate déterministe
            int newState = deterministic.countStates();
            deterministic.addState(newState);

            // Ajouter l'ensemble d'états de destination à la table de mappage
            stateMapping[nextStates] = newState;
          }

          // Ajouter une transition de l'état actuel vers l'état de destination
          deterministic.addTransition(stateMapping[currentStates], symbol, stateMapping[nextStates]);
        }
      }
    }
    return deterministic;
  }

  /**
   * TP n°6
   */

  Automaton Automaton::createMinimalMoore(const Automaton &other)
  {
    assert(other.isValid());
    if (!other.isDeterministic())
    {
      other.createDeterministic(other);
    }

    // Initialiser les groupes d'états.
    std::map<int, int> stateGroups;
    for (auto state : other.states)
    {
      stateGroups[state] = other.isStateFinal(state) ? 1 : 0;
    }

    bool groupsChanged;
    do
    {
      groupsChanged = false;
      std::map<int, int> newStateGroups = stateGroups;

      // Assigner un groupe à chaque état en fonction de ses transitions.
      for (auto state : other.states)
      {
        for (auto alpha : other.alphabet)
        {
          auto it = other.transitions.find(state);
          if (it != other.transitions.end())
          {
            auto transIt = it->second.find(alpha);
            if (transIt != it->second.end() && !transIt->second.empty())
            {
              int targetState = *(transIt->second.begin());
              std::pair<int, int> key = {stateGroups[state], stateGroups[targetState]};
              if (newStateGroups[state] != key.second)
              {
                newStateGroups[state] = key.second;
                groupsChanged = true;
              }
            }
          }
        }
      }

      stateGroups = newStateGroups;
    } while (groupsChanged);

    // Construire le nouvel automate minimal.
    Automaton minimal;
    minimal.alphabet = other.alphabet; // Copier l'alphabet
    std::map<int, int> newStatesMapping;
    int newStateId = 0;
    std::set<int> finalStatesInGroups;

    // Identifier les états finaux dans chaque groupe
    for (const auto &[state, group] : stateGroups)
    {
      if (other.isStateFinal(state))
      {
        finalStatesInGroups.insert(group);
      }
    }

    for (auto group : stateGroups)
    {
      if (newStatesMapping.find(group.second) == newStatesMapping.end())
      {
        newStatesMapping[group.second] = newStateId++;
        minimal.addState(newStatesMapping[group.second]);
        if (finalStatesInGroups.find(group.second) != finalStatesInGroups.end())
        {
          minimal.setStateFinal(newStatesMapping[group.second]);
        }
        if (other.isStateInitial(group.first))
        {
          minimal.setStateInitial(newStatesMapping[group.second]);
        }
      }
    }

    // Recréer les transitions pour le nouvel automate.
    for (auto state : other.states)
    {
      for (auto alpha : other.alphabet)
      {
        auto it = other.transitions.find(state);
        if (it != other.transitions.end())
        {
          auto transIt = it->second.find(alpha);
          if (transIt != it->second.end() && !transIt->second.empty())
          {
            int fromState = newStatesMapping[stateGroups[state]];
            int toState = newStatesMapping[stateGroups[*(transIt->second.begin())]];
            minimal.addTransition(fromState, alpha, toState);
          }
        }
      }
    }

    return minimal;
  }

  Automaton Automaton::createMinimalBrzozowski(const Automaton &other)
  {
    assert(other.isValid());
    Automaton minimalBrzozowski = createMirror(other);
    minimalBrzozowski = createDeterministic(minimalBrzozowski);
    minimalBrzozowski = createMirror(minimalBrzozowski);
    minimalBrzozowski = createDeterministic(minimalBrzozowski);
    minimalBrzozowski = createComplete(minimalBrzozowski);
    return minimalBrzozowski;
  }

} // namespace fa