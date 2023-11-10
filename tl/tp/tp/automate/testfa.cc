#include "gtest/gtest.h"

#include "Automaton.h"

namespace fa
{

    class AutomatonTest : public ::testing::Test
    {
    protected:
        Automaton automaton;
        Automaton notAnAutomaton;
        Automaton forPrint;
    };

    // Test TP1 *******************************************************************************************************

    TEST_F(AutomatonTest, TestIsValid_TRUE)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.setStateInitial(0);
        automaton.setStateFinal(1);
        automaton.addTransition(0, 'a', 1);

        EXPECT_TRUE(automaton.isValid());
    }

    TEST_F(AutomatonTest, TestIsValid_FALSE)
    {
        EXPECT_FALSE(notAnAutomaton.isValid());
    }

    // test is valid sur toute lettre ASCII possedant une représentation graphique
    TEST_F(AutomatonTest, TestIsValid_TRUE_ASCII)
    {
        automaton.addState(0);
        for (char c = 33; c < 127; ++c)
        {
            automaton.addSymbol(c);
        }

        EXPECT_TRUE(automaton.isValid());
    }

    TEST_F(AutomatonTest, TestHasSymbol_TRUE)
    {
        automaton.addSymbol('a');
        EXPECT_TRUE(automaton.hasSymbol('a'));
    }

    TEST_F(AutomatonTest, TestHasSymbol_FALSE_NoSymbolAdded)
    {
        EXPECT_FALSE(automaton.hasSymbol('a'));
    }

    TEST_F(AutomatonTest, TestHasSymbol_FALSE_InvalidChar)
    {
        automaton.addState(0);
        automaton.addState(1);
        automaton.addTransition(0, fa::Epsilon, 1);
        EXPECT_FALSE(automaton.hasSymbol(fa::Epsilon));
    }

    TEST_F(AutomatonTest, TestHasSymbol_FALSE)
    {
        automaton.addSymbol('a');
        EXPECT_FALSE(automaton.hasSymbol('c'));
    }

    TEST_F(AutomatonTest, TestAddSymbol_TRUE)
    {
        EXPECT_TRUE(automaton.addSymbol('c'));
    }

    TEST_F(AutomatonTest, TestAddSymbol_FALSE_InvalidChar)
    {
        EXPECT_FALSE(automaton.addSymbol('\0'));
        EXPECT_FALSE(automaton.addSymbol(fa::Epsilon));
    }

    TEST_F(AutomatonTest, TestAddSymbol_FALSE_AlreadyPresent)
    {
        automaton.addSymbol('a');
        EXPECT_FALSE(automaton.addSymbol('a'));
    }

    TEST_F(AutomatonTest, TestRemoveSymbol_TRUE)
    {
        automaton.addSymbol('a');
        EXPECT_TRUE(automaton.removeSymbol('a'));
        EXPECT_FALSE(automaton.hasSymbol('a'));
    }

    TEST_F(AutomatonTest, TestRemoveSymbol_FALSE_InvalidChar)
    {
        automaton.addSymbol('a');
        EXPECT_FALSE(automaton.removeSymbol('\0'));
        EXPECT_FALSE(automaton.removeSymbol(fa::Epsilon));
        EXPECT_TRUE(automaton.hasSymbol('a'));
    }

    TEST_F(AutomatonTest, TestRemoveSymbol_FALSE_NotPresent)
    {
        automaton.addSymbol('a');
        EXPECT_FALSE(automaton.removeSymbol('c'));
        EXPECT_TRUE(automaton.hasSymbol('a'));
    }

    TEST_F(AutomatonTest, TestCountSymbols)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        std::size_t expectedCount = 2;
        EXPECT_EQ(expectedCount, automaton.countSymbols());
    }

    TEST_F(AutomatonTest, TestHasState_TRUE)
    {
        automaton.addState(0);
        EXPECT_TRUE(automaton.hasState(0));
    }

    TEST_F(AutomatonTest, TestHasState_FALSE)
    {
        EXPECT_FALSE(automaton.hasState(2));
    }

    TEST_F(AutomatonTest, TestAddState_TRUE)
    {
        EXPECT_TRUE(automaton.addState(2));
    }

    TEST_F(AutomatonTest, TestAddState_FALSE_AlreadyPresent)
    {
        automaton.addState(0);
        EXPECT_FALSE(automaton.addState(0));
    }

    TEST_F(AutomatonTest, TestRemoveState_TRUE)
    {
        automaton.addState(0);
        EXPECT_TRUE(automaton.removeState(0));
        EXPECT_FALSE(automaton.hasState(0));
    }

    TEST_F(AutomatonTest, TestRemoveState_FALSE_NotPresent)
    {
        EXPECT_FALSE(automaton.removeState(2));
    }

    TEST_F(AutomatonTest, TestRemoveState_TRUE_StateOnInitialState)
    {
        automaton.addState(0);
        automaton.setStateInitial(0);
        EXPECT_TRUE(automaton.removeState(0));
        EXPECT_FALSE(automaton.hasState(0));
    }

    TEST_F(AutomatonTest, TestRemoveState_TRUE_StateOnFinalState)
    {
        automaton.addState(0);
        automaton.setStateFinal(0);
        EXPECT_TRUE(automaton.removeState(0));
        EXPECT_FALSE(automaton.hasState(0));
    }

    TEST_F(AutomatonTest, TestRemoveState_TRUE_StateTransition)
    {
        automaton.addSymbol('a');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(1, 'a', 0);
        automaton.addTransition(0, 'a', 0);
        automaton.addTransition(1, 'a', 1);
        EXPECT_TRUE(automaton.removeState(0));
        EXPECT_FALSE(automaton.hasState(0));
        EXPECT_FALSE(automaton.hasTransition(0, 'a', 1));
        EXPECT_FALSE(automaton.hasTransition(1, 'a', 0));
        EXPECT_FALSE(automaton.hasTransition(0, 'a', 0));
        EXPECT_TRUE(automaton.hasTransition(1, 'a', 1));
    }
    TEST_F(AutomatonTest, TestCountStates)
    {
        automaton.addState(0);
        automaton.addState(1);
        std::size_t expectedCount = 2;
        EXPECT_EQ(expectedCount, automaton.countStates());
    }

    TEST_F(AutomatonTest, TestSetAndGetStateInitial)
    {
        automaton.addState(1);
        automaton.addState(0);
        automaton.setStateInitial(1);
        EXPECT_TRUE(automaton.isStateInitial(1));
        EXPECT_FALSE(automaton.isStateInitial(0));
    }

    TEST_F(AutomatonTest, TestSetAndGetStateFinal)
    {
        automaton.addState(1);
        automaton.addState(0);
        automaton.setStateFinal(0);
        EXPECT_TRUE(automaton.isStateFinal(0));
        EXPECT_FALSE(automaton.isStateFinal(1));
    }

    TEST_F(AutomatonTest, TestAddTransition_TRUE)
    {
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        EXPECT_TRUE(automaton.addTransition(0, 'b', 1));
        EXPECT_TRUE(automaton.hasTransition(0, 'b', 1));
    }

    TEST_F(AutomatonTest, TestAddTransition_FALSE_AlreadyPresent)
    {
        automaton.addSymbol('a');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addTransition(0, 'a', 1);
        EXPECT_FALSE(automaton.addTransition(0, 'a', 1));
    }

    TEST_F(AutomatonTest, TestAddTransition_FALSE_StateNotPresent)
    {
        automaton.addSymbol('a');
        automaton.addState(0);
        EXPECT_FALSE(automaton.addTransition(0, 'a', 2));
        EXPECT_FALSE(automaton.addTransition(2, 'a', 0));
    }

    TEST_F(AutomatonTest, TestAddTransition_FALSE_SymbolNotPresent)
    {
        automaton.addState(0);
        automaton.addState(1);
        EXPECT_FALSE(automaton.addTransition(0, 'c', 1));
    }

    TEST_F(AutomatonTest, TestAddTransition_TRUE_SameState)
    {
        automaton.addSymbol('a');
        automaton.addState(0);
        EXPECT_TRUE(automaton.addTransition(0, 'a', 0));
    }

    TEST_F(AutomatonTest, TestAddTransition_TRUE_EPSILON)
    {
        automaton.addState(0);
        automaton.addState(1);
        EXPECT_TRUE(automaton.addTransition(0, fa::Epsilon, 1));
    }

    TEST_F(AutomatonTest, TestRemoveTransition_TRUE)
    {
        automaton.addSymbol('a');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addTransition(0, 'a', 1);
        EXPECT_TRUE(automaton.removeTransition(0, 'a', 1));
    }

    TEST_F(AutomatonTest, TestRemoveTransition_FALSE_NotPresent)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addTransition(0, 'a', 1);
        automaton.removeTransition(0, 'b', 1);
        EXPECT_TRUE(automaton.hasTransition(0, 'a', 1));
        int transition = automaton.countTransitions();
        EXPECT_EQ(1, transition);
    }

    TEST_F(AutomatonTest, TestRemoveTransition_TRUE_EpsilonTranstion)
    {
        automaton.addSymbol('a');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addTransition(0, fa::Epsilon, 1);
        automaton.removeTransition(0, fa::Epsilon, 1);
        automaton.prettyPrint(std::cout);
        EXPECT_FALSE(automaton.hasEpsilonTransition());
        int transition = automaton.countTransitions();
        EXPECT_EQ(0, transition);
    }

    TEST_F(AutomatonTest, TestCountTransitions)
    {
        const size_t expectCount0 = 0;
        const size_t expectCount1 = 1;
        const size_t expectCount2 = 2;

        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        EXPECT_EQ(expectCount0, automaton.countTransitions());
        automaton.addTransition(0, 'a', 1);
        EXPECT_EQ(expectCount1, automaton.countTransitions());
        automaton.addTransition(1, 'b', 0);
        EXPECT_EQ(expectCount2, automaton.countTransitions());
    }

    TEST_F(AutomatonTest, TestHasTransition_TRUE)
    {
        automaton.addSymbol('a');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addTransition(0, 'a', 1);
        EXPECT_TRUE(automaton.hasTransition(0, 'a', 1));
    }

    TEST_F(AutomatonTest, TestHasTransition_FALSE_NotPresent)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addTransition(0, 'a', 1);
        EXPECT_FALSE(automaton.hasTransition(0, 'b', 1));
    }

    TEST_F(AutomatonTest, TestHasTransition_FALSE_transitionEmpty)
    {
        EXPECT_FALSE(automaton.hasTransition(0, 'a', 1));
    }

    TEST_F(AutomatonTest, TestHasTransition_FALSE_StateNotPresent)
    {
        automaton.addSymbol('a');
        automaton.addState(0);
        automaton.addTransition(0, 'a', 2);
        EXPECT_FALSE(automaton.hasTransition(0, 'a', 2));
    }

    TEST_F(AutomatonTest, TestHasTransition_FALSE_SymbolNotPresent)
    {
        automaton.addSymbol('a');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addTransition(0, 'c', 1);
        EXPECT_FALSE(automaton.hasTransition(0, 'c', 1));
    }

    TEST_F(AutomatonTest, TestPrettyPrint)
    {
        forPrint.addSymbol('a');
        forPrint.addSymbol('b');
        forPrint.addSymbol('c');
        forPrint.addState(0);
        forPrint.addState(1);
        forPrint.addState(2);
        forPrint.setStateInitial(0);
        forPrint.setStateFinal(2);
        forPrint.addTransition(0, 'a', 1);
        forPrint.addTransition(1, 'b', 1);
        forPrint.addTransition(1, 'c', 2);
        forPrint.addTransition(1, '\0', 2);
        // Effectuez la sortie de la fonction prettyPrint directement ici.
        forPrint.prettyPrint(std::cout);
    }

    // Test TP2 *******************************************************************************************************

    TEST_F(AutomatonTest, TestHasEpsilonTransition_TRUE_usingHasTransition)
    {
        automaton.addSymbol('a');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addTransition(0, fa::Epsilon, 1);
        EXPECT_TRUE(automaton.hasTransition(0, fa::Epsilon, 1));
    }

    TEST_F(AutomatonTest, TestHasEpsilonTransition_TRUE_usingHasEpsilonTransition)
    {
        automaton.addSymbol('a');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addTransition(0, fa::Epsilon, 1);
        EXPECT_TRUE(automaton.hasEpsilonTransition());
    }

    TEST_F(AutomatonTest, TestHasEpsilonTransition_FALSE)
    {
        automaton.addSymbol('a');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addTransition(0, 'a', 1);
        EXPECT_FALSE(automaton.hasEpsilonTransition());
    }

    TEST_F(AutomatonTest, TestIsDeterministic_TRUE)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.setStateInitial(0);
        automaton.setStateFinal(1);

        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(0, 'b', 0); // Une seule transition possible pour 'b'
        automaton.addTransition(1, 'a', 1);
        automaton.addTransition(1, 'b', 1);

        EXPECT_TRUE(automaton.isDeterministic());
    }

    TEST_F(AutomatonTest, TestIsDeterministic_FALSE_DueToMultipleTransitions)
    {
        automaton.addSymbol('a');
        automaton.addState(0);
        automaton.addState(1);
        automaton.setStateInitial(0);
        automaton.setStateFinal(1);
        automaton.addTransition(0, 'a', 0);
        automaton.addTransition(0, 'a', 1);
        EXPECT_FALSE(automaton.isDeterministic());
    }

    TEST_F(AutomatonTest, TestIsDeterministic_FALSE_NoInitialState)
    {
        automaton.addSymbol('a');
        automaton.addState(0);
        automaton.addState(1);
        automaton.setStateFinal(1);
        automaton.addTransition(0, 'a', 1);
        EXPECT_FALSE(automaton.isDeterministic());
    }

    TEST_F(AutomatonTest, TestIsDeterministic_FALSE_MultipleInitialState)
    {
        automaton.addSymbol('a');
        automaton.addState(0);
        automaton.addState(1);
        automaton.setStateInitial(0);
        automaton.setStateInitial(1);
        automaton.setStateFinal(1);
        automaton.addTransition(0, 'a', 1);
        EXPECT_FALSE(automaton.isDeterministic());
    }

    TEST_F(AutomatonTest, TestIsComplete_TRUE)
    {

        // Ajout des symboles à l'alphabet
        automaton.addSymbol('a');
        automaton.addSymbol('b');

        // Ajout des états
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);

        // Définition de l'état initial
        automaton.setStateInitial(0);

        // Définition de l'état final
        automaton.setStateFinal(1);

        // Ajout des transitions
        automaton.addTransition(0, 'a', 1); // Transition de l'état 0 à l'état 1 avec '0'
        automaton.addTransition(0, 'b', 2); // Transition de l'état 0 à l'état 2 avec '1'
        automaton.addTransition(1, 'a', 2); // Transition de l'état 1 à l'état 2 avec '0'
        automaton.addTransition(1, 'b', 1); // Transition de l'état 1 à lui-même avec '1'
        automaton.addTransition(2, 'a', 2); // Transition de l'état 2 à lui-même avec '0'
        automaton.addTransition(2, 'b', 2); // Transition de l'état 2 à lui-même avec '1'

        EXPECT_TRUE(automaton.isComplete());
    }

    TEST_F(AutomatonTest, TestIsComplete_FALSE_TransitionMissing)
    {
        // Ajout des symboles à l'alphabet
        automaton.addSymbol('a');
        automaton.addSymbol('b');

        // Ajout des états
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);

        // Définition de l'état initial
        automaton.setStateInitial(0);

        // Définition de l'état final
        automaton.setStateFinal(1);

        // Ajout des transitions
        automaton.addTransition(0, 'a', 1); // Transition de l'état 0 à l'état 1 avec '0'
        automaton.addTransition(0, 'b', 2); // Transition de l'état 0 à l'état 2 avec '1'
        automaton.addTransition(2, 'b', 2); // Transition de l'état 2 à lui-même avec '1'
        EXPECT_FALSE(automaton.isComplete());
    }

    TEST_F(AutomatonTest, TestIsComplete_TRUE_NoInitialState)
    {
        // Ajout des symboles à l'alphabet
        automaton.addSymbol('a');
        automaton.addSymbol('b');

        // Ajout des états
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);

        // Définition de l'état final
        automaton.setStateFinal(1);

        // Ajout des transitions
        automaton.addTransition(0, 'a', 1); // Transition de l'état 0 à l'état 1 avec '0'
        automaton.addTransition(0, 'b', 2); // Transition de l'état 0 à l'état 2 avec '1'
        automaton.addTransition(1, 'a', 2); // Transition de l'état 1 à l'état 2 avec '0'
        automaton.addTransition(1, 'b', 1); // Transition de l'état 1 à lui-même avec '1'
        automaton.addTransition(2, 'a', 2); // Transition de l'état 2 à lui-même avec '0'
        automaton.addTransition(2, 'b', 2); // Transition de l'état 2 à lui-même avec '1'
        EXPECT_TRUE(automaton.isComplete());
    }

    TEST_F(AutomatonTest, TestIsComplete_TRUE_MultipleInitialState)
    {
        // Ajout des symboles à l'alphabet
        automaton.addSymbol('a');
        automaton.addSymbol('b');

        // Ajout des états
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);

        // Définition de l'état initial
        automaton.setStateInitial(0);
        automaton.setStateInitial(1);

        // Définition de l'état final
        automaton.setStateFinal(1);

        // Ajout des transitions
        automaton.addTransition(0, 'a', 1); // Transition de l'état 0 à l'état 1 avec '0'
        automaton.addTransition(0, 'b', 2); // Transition de l'état 0 à l'état 2 avec '1'
        automaton.addTransition(1, 'a', 2); // Transition de l'état 1 à l'état 2 avec '0'
        automaton.addTransition(1, 'b', 1); // Transition de l'état 1 à lui-même avec '1'
        automaton.addTransition(2, 'a', 2); // Transition de l'état 2 à lui-même avec '0'
        automaton.addTransition(2, 'b', 2); // Transition de l'état 2 à lui-même avec '1'
        EXPECT_TRUE(automaton.isComplete());
    }

    TEST_F(AutomatonTest, TestIsComplete_TRUE_MultipleTransition)
    {
        // Ajout des symboles à l'alphabet
        automaton.addSymbol('a');
        automaton.addSymbol('b');

        // Ajout des états
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);

        // Définition de l'état initial
        automaton.setStateInitial(0);

        // Définition de l'état final
        automaton.setStateFinal(1);

        // Ajout des transitions
        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(0, 'a', 2);
        automaton.addTransition(0, 'b', 2);
        automaton.addTransition(1, 'a', 2);
        automaton.addTransition(1, 'b', 1);
        automaton.addTransition(2, 'a', 2);
        automaton.addTransition(2, 'b', 2);
        EXPECT_TRUE(automaton.isComplete());
    }

    // test for createComplete()
    TEST_F(AutomatonTest, TestCreateComplete_AlreadyComplete)
    {
        Automaton completeAutomaton;

        // Ajout des symboles à l'alphabet
        automaton.addSymbol('0');
        automaton.addSymbol('1');

        // Ajout des états
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);

        // Définition de l'état initial
        automaton.setStateInitial(0);

        // Définition de l'état final
        automaton.setStateFinal(1);

        // Ajout des transitions
        automaton.addTransition(0, '0', 1); // Transition de l'état 0 à l'état 1 avec '0'
        automaton.addTransition(0, '1', 2); // Transition de l'état 0 à l'état 2 avec '1'
        automaton.addTransition(1, '0', 2); // Transition de l'état 1 à l'état 2 avec '0'
        automaton.addTransition(1, '1', 1); // Transition de l'état 1 à lui-même avec '1'
        automaton.addTransition(2, '0', 2); // Transition de l'état 2 à lui-même avec '0'
        automaton.addTransition(2, '1', 2); // Transition de l'état 2 à lui-même avec '1'

        completeAutomaton = automaton.createComplete(automaton);
        EXPECT_TRUE(completeAutomaton.isComplete());
        EXPECT_TRUE(automaton.isComplete());
        EXPECT_EQ(automaton.countStates(), completeAutomaton.countStates());
        EXPECT_EQ(automaton.countSymbols(), completeAutomaton.countSymbols());
        EXPECT_EQ(automaton.countTransitions(), completeAutomaton.countTransitions());
    }

    TEST_F(AutomatonTest, TestCreateComplete_TransitionMissing)
    {
        Automaton completeAutomaton;

        // Ajout des symboles à l'alphabet
        automaton.addSymbol('0');
        automaton.addSymbol('1');

        // Ajout des états
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);

        // Définition de l'état initial
        automaton.setStateInitial(0);

        // Définition de l'état final
        automaton.setStateFinal(1);

        // Ajout des transitions
        automaton.addTransition(0, '0', 1); // Transition de l'état 0 à l'état 1 avec '0'
        automaton.addTransition(0, '1', 2); // Transition de l'état 0 à l'état 2 avec '1'
        automaton.addTransition(2, '1', 2); // Transition de l'état 2 à lui-même avec '1'
        completeAutomaton = automaton.createComplete(automaton);
        EXPECT_TRUE(completeAutomaton.isComplete());
    }

    TEST_F(AutomatonTest, TestCreateComplete_NoInitialState)
    {
        Automaton completeAutomaton;

        // Ajout des symboles à l'alphabet
        automaton.addSymbol('0');
        automaton.addSymbol('1');

        // Ajout des états
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);

        // Définition de l'état final
        automaton.setStateFinal(1);

        // Ajout des transitions
        automaton.addTransition(0, '0', 1); // Transition de l'état 0 à l'état 1 avec '0'
        automaton.addTransition(0, '1', 2); // Transition de l'état 0 à l'état 2 avec '1'
        automaton.addTransition(1, '0', 2); // Transition de l'état 1 à l'état 2 avec '0'
        automaton.addTransition(1, '1', 1); // Transition de l'état 1 à lui-même avec '1'
        automaton.addTransition(2, '0', 2); // Transition de l'état 2 à lui-même avec '0'
        automaton.addTransition(2, '1', 2); // Transition de l'état 2 à lui-même avec '1'

        completeAutomaton = automaton.createComplete(automaton);
        EXPECT_TRUE(completeAutomaton.isComplete());
    }

    TEST_F(AutomatonTest, TestCreateComplete_MultipleInitialState)
    {
        Automaton completeAutomaton;

        // Ajout des symboles à l'alphabet
        automaton.addSymbol('0');
        automaton.addSymbol('1');

        // Ajout des états
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);

        // Définition de l'état initial
        automaton.setStateInitial(0);
        automaton.setStateInitial(1);

        // Définition de l'état final
        automaton.setStateFinal(1);

        // Ajout des transitions
        automaton.addTransition(0, '0', 1); // Transition de l'état 0 à l'état 1 avec '0'
        automaton.addTransition(0, '1', 2); // Transition de l'état 0 à l'état 2 avec '1'
        automaton.addTransition(1, '0', 2); // Transition de l'état 1 à l'état 2 avec '0'
        automaton.addTransition(1, '1', 1); // Transition de l'état 1 à lui-même avec '1'
        automaton.addTransition(2, '0', 2); // Transition de l'état 2 à lui-même avec '0'
        automaton.addTransition(2, '1', 2); // Transition de l'état 2 à lui-même avec '1'

        completeAutomaton = automaton.createComplete(automaton);
        EXPECT_TRUE(completeAutomaton.isComplete());
    }

    // Test pour createMirror()
    TEST_F(AutomatonTest, TestCreateMirror)
    {
        // init automaton
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);
        automaton.setStateInitial(0);
        automaton.setStateFinal(2);
        automaton.addTransition(0, 'a', 1); // #1
        automaton.addTransition(0, 'a', 2); // #2
        automaton.addTransition(1, 'b', 2); // #3
        automaton.addTransition(2, 'b', 2); // #4
        automaton.addTransition(2, 'b', 0); // #5

        // creating mirror of automaton
        Automaton mirrorAutomaton;
        mirrorAutomaton = automaton.createMirror(automaton);

        // test
        EXPECT_TRUE(mirrorAutomaton.hasSymbol('a'));
        EXPECT_TRUE(mirrorAutomaton.hasSymbol('b'));
        EXPECT_TRUE(mirrorAutomaton.hasState(0));
        EXPECT_TRUE(mirrorAutomaton.hasState(1));
        EXPECT_TRUE(mirrorAutomaton.hasState(2));
        EXPECT_TRUE(mirrorAutomaton.isStateInitial(2));
        EXPECT_TRUE(mirrorAutomaton.isStateFinal(0));
        EXPECT_TRUE(mirrorAutomaton.hasTransition(1, 'a', 0)); // #1
        EXPECT_TRUE(mirrorAutomaton.hasTransition(2, 'a', 0)); // #2
        EXPECT_TRUE(mirrorAutomaton.hasTransition(2, 'b', 1)); // #3
        EXPECT_TRUE(mirrorAutomaton.hasTransition(2, 'b', 2)); // #4
        EXPECT_TRUE(mirrorAutomaton.hasTransition(0, 'b', 2)); // #5
    }

    // Test creation d'un mirroir de mirroir et donc retour à l'origine
    TEST_F(AutomatonTest, TestCreateMirrorOfMirror)
    {

        // init automaton
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);
        automaton.setStateInitial(0);
        automaton.setStateFinal(2);
        automaton.addTransition(0, 'a', 1); // #1
        automaton.addTransition(0, 'a', 2); // #2
        automaton.addTransition(1, 'b', 2); // #3
        automaton.addTransition(2, 'b', 2); // #4
        automaton.addTransition(2, 'b', 0); // #5

        // creating mirror of automaton
        Automaton mirrorAutomaton;
        mirrorAutomaton = automaton.createMirror(automaton);

        // creating mirror of mirrorAutomaton
        Automaton mirrorOfMirrorAutomaton;
        mirrorOfMirrorAutomaton = mirrorAutomaton.createMirror(mirrorAutomaton);

        // test
        EXPECT_TRUE(mirrorOfMirrorAutomaton.hasSymbol('a'));
        EXPECT_TRUE(mirrorOfMirrorAutomaton.hasSymbol('b'));
        EXPECT_TRUE(mirrorOfMirrorAutomaton.hasState(0));
        EXPECT_TRUE(mirrorOfMirrorAutomaton.hasState(1));
        EXPECT_TRUE(mirrorOfMirrorAutomaton.hasState(2));
        EXPECT_TRUE(mirrorOfMirrorAutomaton.isStateInitial(0));
        EXPECT_TRUE(mirrorOfMirrorAutomaton.isStateFinal(2));
        EXPECT_TRUE(mirrorOfMirrorAutomaton.hasTransition(0, 'a', 1)); // #1
        EXPECT_TRUE(mirrorOfMirrorAutomaton.hasTransition(0, 'a', 2)); // #2
        EXPECT_TRUE(mirrorOfMirrorAutomaton.hasTransition(1, 'b', 2)); // #3
        EXPECT_TRUE(mirrorOfMirrorAutomaton.hasTransition(2, 'b', 2)); // #4
        EXPECT_TRUE(mirrorOfMirrorAutomaton.hasTransition(2, 'b', 0)); // #5
    }

    // Test pour makeTransition()
    TEST_F(AutomatonTest, TestMakeTransition)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(1);
        automaton.addState(2);
        automaton.addTransition(1, 'a', 2);

        std::set<int> originState = {1};
        std::set<int> resultState = automaton.makeTransition(originState, 'a');
        EXPECT_TRUE(resultState.count(2) > 0);
    }

    TEST_F(AutomatonTest, TestMakeTransition_transitionFromOneState)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);
        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(1, 'b', 2);
        automaton.addTransition(2, 'a', 0);

        std::set<int> origin = {0};
        std::set<int> expected = {1};
        std::set<int> result = automaton.makeTransition(origin, 'a');
        EXPECT_EQ(result, expected);
    }

    TEST_F(AutomatonTest, TestMakeTransition_transitionFromMultipleState)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);
        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(1, 'b', 2);
        automaton.addTransition(2, 'a', 0);

        std::set<int> origin = {0, 2};
        std::set<int> expected = {0, 1};
        std::set<int> result = automaton.makeTransition(origin, 'a');
        EXPECT_EQ(result, expected);
    }

    TEST_F(AutomatonTest, TestMakeTransition_transitionWithNoMatchingSymbol)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(1);
        automaton.addState(2);
        automaton.addTransition(1, 'a', 2);

        std::set<int> originState = {1};
        std::set<int> resultState = automaton.makeTransition(originState, 'b');
        EXPECT_TRUE(resultState.empty());
    }

    // Test pour readString()
    TEST_F(AutomatonTest, TestReadString)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);
        automaton.setStateInitial(0);
        automaton.setStateFinal(2);
        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(1, 'b', 2);

        // Définissez la chaîne de caractères à lire
        std::string input = "ab";

        // Appelez la fonction readString avec la chaîne d'entrée
        std::set<int> resultState = automaton.readString(input);

        // Créez un ensemble d'états finaux attendus (dans cet exemple, 2 est l'état final)
        std::set<int> expectedFinalState;
        expectedFinalState.insert(2);

        // Utilisez EXPECT_TRUE pour vérifier si les ensembles d'états sont égaux
        EXPECT_TRUE(resultState == expectedFinalState);
    }

    // Test pour match()
    TEST_F(AutomatonTest, TestMatch_TRUE)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);
        automaton.setStateInitial(0);
        automaton.setStateFinal(2);
        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(1, 'b', 2);

        std::string input = "ab";
        EXPECT_TRUE(automaton.match(input));
    }

    TEST_F(AutomatonTest, TestMatch_FALSE)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(2);
        automaton.addTransition(1, 'a', 2);

        std::string input = "ba";
        EXPECT_FALSE(automaton.match(input));
    }

    // Test TP3 *******************************************************************************************************

    // Test pour isLanguageEmpty()
    TEST_F(AutomatonTest, TestIsLanguageEmpty_TRUE)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.setStateInitial(0);
        automaton.setStateFinal(1);
        automaton.addTransition(0, 'a', 0);
        automaton.addTransition(0, 'b', 0);

        EXPECT_TRUE(automaton.isLanguageEmpty());
    }

    TEST_F(AutomatonTest, TestIsLanguageEmpty_FALSE)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.setStateInitial(0);
        automaton.setStateFinal(1);

        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(0, 'b', 0);
        automaton.addTransition(1, 'a', 1);
        automaton.addTransition(1, 'b', 1);
        automaton.addTransition(1, 'b', 0);

        EXPECT_FALSE(automaton.isLanguageEmpty());
    }

    TEST_F(AutomatonTest, TestIsLanguageEmpty_NoFinalState)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.setStateInitial(0);

        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(0, 'b', 0);
        automaton.addTransition(1, 'a', 1);
        automaton.addTransition(1, 'b', 1);
        automaton.addTransition(1, 'b', 0);

        EXPECT_TRUE(automaton.isLanguageEmpty());
    }

    // test removeNonAccessibleStates
    TEST_F(AutomatonTest, testRemoveAccessibleStates_hasNonAccessibleStates)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);

        automaton.setStateInitial(0);
        automaton.setStateFinal(1);

        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(1, 'b', 0);

        automaton.removeNonAccessibleStates();

        EXPECT_TRUE(automaton.hasState(0));
        EXPECT_TRUE(automaton.hasState(1));
        EXPECT_FALSE(automaton.hasState(2));
    }

    TEST_F(AutomatonTest, testRemoveAccessibleStates_hasNoNonAccessibleStates)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);

        automaton.setStateInitial(0);
        automaton.setStateFinal(1);

        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(1, 'b', 0);

        automaton.removeNonAccessibleStates();

        EXPECT_TRUE(automaton.hasState(0));
        EXPECT_TRUE(automaton.hasState(1));
    }

    // Test when no states are accessible
    TEST_F(AutomatonTest, testRemoveAccessibleStates_NoAccessibleStates)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(1);
        automaton.addState(2);
        automaton.addState(3);

        automaton.removeNonAccessibleStates();

        EXPECT_FALSE(automaton.hasState(1));
        EXPECT_FALSE(automaton.hasState(2));
        EXPECT_FALSE(automaton.hasState(3));
    }

    // Test when all states are accessible
    TEST_F(AutomatonTest, testRemoveAccessibleStates_AllStatesAccessible)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);

        automaton.setStateInitial(0);

        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(1, 'b', 2);
        automaton.addTransition(2, 'a', 0);

        automaton.removeNonAccessibleStates();

        EXPECT_TRUE(automaton.hasState(0));
        EXPECT_TRUE(automaton.hasState(1));
        EXPECT_TRUE(automaton.hasState(2));
    }

    // Test when only some states are accessible
    TEST_F(AutomatonTest, testRemoveAccessibleStates_SomeStatesAccessible)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);
        automaton.addState(3);

        automaton.setStateInitial(0);

        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(1, 'b', 0);
        // State 2 and 3 are not accessible

        automaton.removeNonAccessibleStates();

        EXPECT_TRUE(automaton.hasState(0));
        EXPECT_TRUE(automaton.hasState(1));
        EXPECT_FALSE(automaton.hasState(2));
        EXPECT_FALSE(automaton.hasState(3));
    }

    // test removeNonCoAccessibleStates
    TEST_F(AutomatonTest, testRemoveCoAccessibleStates_hasNonCoAccessibleStates)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);

        automaton.setStateInitial(0);
        automaton.setStateFinal(1);

        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(1, 'b', 0);

        automaton.removeNonCoAccessibleStates();

        EXPECT_TRUE(automaton.hasState(0));
        EXPECT_TRUE(automaton.hasState(1));
    }

    // Test où aucun état n'est co-accessible
    TEST_F(AutomatonTest, testRemoveCoAccessibleStates_noCoAccessibleStates)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);

        automaton.setStateInitial(0);
        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(1, 'b', 2);

        automaton.removeNonCoAccessibleStates();

        EXPECT_FALSE(automaton.hasState(0));
        EXPECT_FALSE(automaton.hasState(1));
        EXPECT_FALSE(automaton.hasState(2));
    }

    // Test où tous les états sont co-accessibles
    TEST_F(AutomatonTest, testRemoveCoAccessibleStates_allCoAccessibleStates)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);

        automaton.setStateInitial(0);
        automaton.setStateFinal(1);

        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(1, 'b', 0);

        automaton.removeNonCoAccessibleStates();

        EXPECT_TRUE(automaton.hasState(0));
        EXPECT_TRUE(automaton.hasState(1));
    }

    // Test avec plusieurs états finaux
    TEST_F(AutomatonTest, testRemoveCoAccessibleStates_multipleFinalStates)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
        automaton.addState(2);

        automaton.setStateInitial(0);
        automaton.setStateFinal(1);
        automaton.setStateFinal(2);

        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(1, 'b', 2);
        automaton.addTransition(2, 'a', 0);

        automaton.removeNonCoAccessibleStates();

        EXPECT_TRUE(automaton.hasState(0));
        EXPECT_TRUE(automaton.hasState(1));
        EXPECT_TRUE(automaton.hasState(2));
    }

    // Test avec un état final non co-accessible
    TEST_F(AutomatonTest, testRemoveCoAccessibleStates_nonCoAccessibleFinalState)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);

        automaton.setStateInitial(0);
        automaton.setStateFinal(1);

        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(1, 'b', 0);

        automaton.removeNonCoAccessibleStates();

        EXPECT_TRUE(automaton.hasState(0));
        EXPECT_TRUE(automaton.hasState(1));
    }

    /* test createIntersection */

    // Test Intersection d'Automates avec des Langages Disjoints
    TEST_F(AutomatonTest, testCreateIntersection_DisjointLanguages)
    {
        Automaton automaton1;
        Automaton automaton2;

        // initialisation de l'automate 1
        automaton1.addSymbol('a');
        automaton1.addState(0);
        automaton1.addState(1);
        automaton1.addState(2);
        automaton1.setStateInitial(0);
        automaton1.setStateFinal(2);
        automaton1.addTransition(0, 'a', 1);
        automaton1.addTransition(1, 'a', 2);
        automaton1.addTransition(2, 'a', 0);

        // initialisation de l'automate 2
        automaton2.addSymbol('b');
        automaton2.addState(0);
        automaton2.addState(1);
        automaton2.addState(2);
        automaton2.setStateInitial(0);
        automaton2.setStateFinal(2);
        automaton2.addTransition(0, 'b', 1);
        automaton2.addTransition(1, 'b', 2);
        automaton2.addTransition(2, 'b', 0);

        // initialisation de l'automate intersection
        Automaton intersectionAutomaton;
        intersectionAutomaton = Automaton::createIntersection(automaton1, automaton2);
        EXPECT_TRUE(intersectionAutomaton.isLanguageEmpty());
        intersectionAutomaton.prettyPrint(std::cout);
        EXPECT_TRUE(intersectionAutomaton.finalStates.empty());
        EXPECT_TRUE(intersectionAutomaton.transitions.empty());
    }

    // Test Intersection d'Automates avec des Langages Non Disjoints
    TEST_F(AutomatonTest, testCreateIntersection_NonDisjointLanguages)
    {
        Automaton automaton1;
        Automaton automaton2;

        // initialisation de l'automate 1
        automaton1.addSymbol('a');
        automaton1.addState(0);
        automaton1.addState(1);
        automaton1.addState(2);
        automaton1.setStateInitial(0);
        automaton1.setStateFinal(2);
        automaton1.addTransition(0, 'a', 1);
        automaton1.addTransition(1, 'a', 2);
        automaton1.addTransition(2, 'a', 0);

        // initialisation de l'automate 2
        automaton2.addSymbol('a');
        automaton2.addState(0);
        automaton2.addState(1);
        automaton2.addState(2);
        automaton2.setStateInitial(0);
        automaton2.setStateFinal(2);
        automaton2.addTransition(0, 'a', 1);
        automaton2.addTransition(1, 'a', 2);
        automaton2.addTransition(2, 'a', 0);

        // initialisation de l'automate intersection
        Automaton intersectionAutomaton;
        intersectionAutomaton = Automaton::createIntersection(automaton1, automaton2);
        EXPECT_FALSE(intersectionAutomaton.isLanguageEmpty());
        intersectionAutomaton.prettyPrint(std::cout);
        EXPECT_TRUE(intersectionAutomaton.finalStates.size() == 2);
        EXPECT_TRUE(intersectionAutomaton.transitions.size() == 6);
    }

} // namespace fa

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
