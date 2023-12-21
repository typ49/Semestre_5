#include "Automaton.h"

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

    // Copier l'alphabet, les états et les transitions
    complement.alphabet = completeAutomaton.alphabet;
    for (auto state : completeAutomaton.states)
    {
      complement.addState(state);
    }
    complement.transitions = completeAutomaton.transitions;

    // Définir les états initiaux
    for (auto initialState : completeAutomaton.initialStates)
    {
      complement.setStateInitial(initialState);
    }

    // Inverser les états finaux et non finaux
    for (auto state : completeAutomaton.states)
    {
      if (!completeAutomaton.isStateFinal(state))
      {
        complement.setStateFinal(state);
      }
      // Les états finaux dans l'automate original sont laissés non finaux
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
    }

    // S'assurer que l'automate reste valide
    if (states.empty() || initialStates.empty())
    {
      int newState = 0;
      while (hasState(newState)) // Assurez-vous que le nouvel état n'existe pas déjà
      {
        newState++;
      }
      addState(newState);
      setStateInitial(newState);
      if (alphabet.empty())
      {
        addSymbol('a'); // Ajouter un symbole par défaut si l'alphabet est vide
      }
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

    Automaton intersectionAutomaton;
    std::map<std::pair<int, int>, int> productStates;
    int newState = 0;

    // Intersection des alphabets
    std::set<char> intersectionAlphabet;
    for (char symbol : lhs.alphabet)
    {
      if (rhs.alphabet.find(symbol) != rhs.alphabet.end())
      {
        intersectionAlphabet.insert(symbol);
      }
    }

    // Si l'intersection des alphabets est vide, retourner un automate avec un état non initial et non final
    if (intersectionAlphabet.empty())
    {
      intersectionAutomaton.addState(0);
      intersectionAutomaton.addSymbol('a'); // Symbole arbitraire pour garantir un alphabet non vide
      return intersectionAutomaton;
    }

    // Utiliser l'alphabet d'intersection
    for (char symbol : intersectionAlphabet)
    {
      intersectionAutomaton.addSymbol(symbol);
    }

    // Créer le produit cartésien des états
    for (int lhsState : lhs.states)
    {
      for (int rhsState : rhs.states)
      {
        productStates[std::make_pair(lhsState, rhsState)] = newState;
        intersectionAutomaton.addState(newState);

        if (lhs.initialStates.find(lhsState) != lhs.initialStates.end() &&
            rhs.initialStates.find(rhsState) != rhs.initialStates.end())
        {
          intersectionAutomaton.setStateInitial(newState);
        }

        if (lhs.finalStates.find(lhsState) != lhs.finalStates.end() &&
            rhs.finalStates.find(rhsState) != rhs.finalStates.end())
        {
          intersectionAutomaton.setStateFinal(newState);
        }

        newState++;
      }
    }

    // Créer les transitions pour les symboles dans l'intersection des alphabets
    for (const auto &lhsStatePair : lhs.transitions)
    {
      for (const auto &lhsSymbolPair : lhsStatePair.second)
      {
        if (intersectionAlphabet.find(lhsSymbolPair.first) != intersectionAlphabet.end())
        {
          for (const auto &rhsStatePair : rhs.transitions)
          {
            const auto rhsSymbolPairIter = rhsStatePair.second.find(lhsSymbolPair.first);
            if (rhsSymbolPairIter != rhsStatePair.second.end())
            {
              for (int lhsToState : lhsSymbolPair.second)
              {
                for (int rhsToState : rhsSymbolPairIter->second)
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
    assert(isValid());
    assert(other.isValid());

    // Créer des copies déterministes des automates si nécessaire
    Automaton deterministicThis = isDeterministic() ? *this : createDeterministic(*this);
    Automaton deterministicOther = other.isDeterministic() ? other : createDeterministic(other);

    // Ajouter les symboles manquants à l'autre automate pour uniformiser les alphabets
    for (char symbol : deterministicThis.alphabet)
    {
      if (deterministicOther.alphabet.find(symbol) == deterministicOther.alphabet.end())
      {
        deterministicOther.addSymbol(symbol);
      }
    }

    // Créer le complément de l'autre automate
    Automaton complementOther = createComplement(deterministicOther);

    // Intersection de 'this' et du complément de 'other'
    Automaton intersection = createIntersection(deterministicThis, complementOther);

    // Vérifier si l'intersection est vide
    return intersection.isLanguageEmpty();
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

    if (other.isLanguageEmpty())
    {
      Automaton minimal;
      minimal.alphabet = other.alphabet;
      minimal.addState(0);
      minimal.setStateInitial(0);
      for (auto symbol : other.alphabet)
      {
        minimal.addTransition(0, symbol, 0);
      }
      return minimal;
    }
    // Assurez-vous que l'automate est déterministe et complet
    Automaton deterministic = other.isDeterministic() ? other : Automaton::createDeterministic(other);
    deterministic = deterministic.isComplete() ? deterministic : Automaton::createComplete(deterministic);

    // Supprimez les états non accessibles pour simplifier la minimisation
    deterministic.removeNonAccessibleStates();

    // Création de l'automate minimal
    Automaton minimal;
    minimal.alphabet = deterministic.alphabet;

    // Initialisation des ensembles d'états finaux et non finaux
    std::set<int> finalStates = deterministic.finalStates;
    std::set<int> nonFinalStates;
    for (const auto &state : deterministic.states)
    {
      if (finalStates.find(state) == finalStates.end())
      {
        nonFinalStates.insert(state);
      }
    }

    // Partition initiale des états
    std::vector<std::set<int>> partitions = {finalStates, nonFinalStates};

    // Processus de partitionnement des états
    bool partitionChanged;
    std::map<int, int> stateMapping;
    int newState = 0;
    do
    {
      partitionChanged = false;
      std::vector<std::set<int>> newPartitions;

      for (const auto &partition : partitions)
      {
        std::map<std::string, std::set<int>> signatureToStates;

        for (int state : partition)
        {
          std::string signature;
          std::set<int> stateSet = {state};
          for (char symbol : deterministic.alphabet)
          {
            auto dest = deterministic.makeTransition(stateSet, symbol);
            for (int d : dest)
            {
              signature += std::to_string(d) + ",";
            }
          }
          signatureToStates[signature].insert(state);
        }

        for (const auto &[_, group] : signatureToStates)
        {
          if (group.size() < partition.size())
          {
            partitionChanged = true;
          }
          newPartitions.push_back(group);

          // Vérifie si l'un des états de la partition est un état initial ou final
          bool isInitialState = std::any_of(group.begin(), group.end(), [&](int oldState)
                                            { return deterministic.initialStates.count(oldState) > 0; });
          bool isFinalState = std::any_of(group.begin(), group.end(), [&](int oldState)
                                          { return deterministic.finalStates.count(oldState) > 0; });

          // Ajout de l'état correspondant dans l'automate minimal
          minimal.addState(newState);
          if (isInitialState)
          {
            minimal.setStateInitial(newState);
          }
          if (isFinalState)
          {
            minimal.setStateFinal(newState);
          }

          // Mise à jour du mapping des états
          for (int oldState : group)
          {
            stateMapping[oldState] = newState;
          }
          newState++;
        }
      }

      partitions = std::move(newPartitions);
    } while (partitionChanged);

    // Ajout des transitions
    for (const auto &[from, transMap] : deterministic.transitions)
    {
      for (const auto &[symbol, toSet] : transMap)
      {
        for (int to : toSet)
        {
          if (stateMapping.find(from) != stateMapping.end() &&
              stateMapping.find(to) != stateMapping.end())
          {
            minimal.addTransition(stateMapping[from], symbol, stateMapping[to]);
          }
        }
      }
    }

    // Gestion du cas où l'automate minimal serait vide
    if (minimal.states.empty())
    {
      minimal.addState(0);
      minimal.setStateInitial(0);
      minimal.setStateFinal(0);
    }

    minimal = Automaton::createDeterministic(minimal);
    minimal = Automaton::createComplete(minimal);

    return minimal;
  }

  Automaton Automaton::createMinimalBrzozowski(const Automaton &other)
  {
    assert(other.isValid());
    if (other.isLanguageEmpty())
    {
      Automaton minimal;
      minimal.alphabet = other.alphabet;
      minimal.addState(0);
      minimal.setStateInitial(0);
      for (auto symbol : other.alphabet)
      {
        minimal.addTransition(0, symbol, 0);
      }
      return minimal;
    }

    Automaton minimalBrzozowski = createMirror(other);
    minimalBrzozowski = createDeterministic(minimalBrzozowski);
    minimalBrzozowski = createMirror(minimalBrzozowski);
    minimalBrzozowski = createDeterministic(minimalBrzozowski);
    minimalBrzozowski = createComplete(minimalBrzozowski);
    return minimalBrzozowski;
  }

} // namespace fa