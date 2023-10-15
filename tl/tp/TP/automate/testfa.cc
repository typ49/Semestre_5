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

    TEST_F(AutomatonTest, TestHasSymbol_TRUE)
    {
        automaton.addSymbol('a');
        EXPECT_TRUE(automaton.hasSymbol('a'));
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
    }

    TEST_F(AutomatonTest, TestRemoveSymbol_FALSE_InvalidChar)
    {
        EXPECT_FALSE(automaton.removeSymbol('\0'));
    }

    TEST_F(AutomatonTest, TestRemoveSymbol_FALSE_NotPresent)
    {
        EXPECT_FALSE(automaton.removeSymbol('c'));
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
    }

    TEST_F(AutomatonTest, TestRemoveState_FALSE_NotPresent)
    {
        EXPECT_FALSE(automaton.removeState(2));
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
        automaton.addState(0);
        automaton.addState(1);
        EXPECT_FALSE(automaton.removeTransition(0, 'b', 1));
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
        EXPECT_FALSE(automaton.hasTransition(0, 'a', 2));
    }

    TEST_F(AutomatonTest, TestHasTransition_FALSE_SymbolNotPresent)
    {
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
        automaton.addState(0);
        automaton.addState(1);
        automaton.addTransition(0, fa::Epsilon, 1);
        automaton.prettyPrint(std::cout);
        EXPECT_TRUE(automaton.hasTransition(0, fa::Epsilon, 1));
    }

    TEST_F(AutomatonTest, TestHasEpsilonTransition_FALSE)
    {
        EXPECT_FALSE(automaton.hasEpsilonTransition());
    }

    TEST_F(AutomatonTest, TestIsDeterministic_TRUE)
    {
        EXPECT_TRUE(automaton.isDeterministic());
    }

    TEST_F(AutomatonTest, TestIsDeterministic_FALSE_DueToMultipleTransitions)
    {
        automaton.addTransition(0, 'a', 0);
        EXPECT_FALSE(automaton.isDeterministic());
    }

    TEST_F(AutomatonTest, TestIsDeterministic_FALSE_NoInitialState)
    {
        automaton.removeState(0);
        EXPECT_FALSE(automaton.isDeterministic());
    }

    TEST_F(AutomatonTest, TestIsDeterministic_FALSE_MultipleInitialState)
    {
        automaton.setStateInitial(1);
        EXPECT_FALSE(automaton.isDeterministic());
    }

    TEST_F(AutomatonTest, TestIsDeterministic_FALSE_WithEpsilonTransition)
    {
        automaton.addTransition(0, fa::Epsilon, 1);
        EXPECT_FALSE(automaton.isDeterministic());
    }

    TEST_F(AutomatonTest, TestIsComplete_TRUE)
    {
        automaton.addTransition(0, 'b', 1);
        EXPECT_TRUE(automaton.isComplete());
    }

    TEST_F(AutomatonTest, TestIsComplete_FALSE)
    {
        EXPECT_FALSE(automaton.isComplete());
    }

    TEST_F(AutomatonTest, TestIsComplete_FALSE_NoInitialState)
    {
        automaton.removeState(0);
        EXPECT_FALSE(automaton.isComplete());
    }

    TEST_F(AutomatonTest, TestIsComplete_FALSE_MultipleInitialState)
    {
        automaton.setStateInitial(1);
        EXPECT_FALSE(automaton.isComplete());
    }

    TEST_F(AutomatonTest, TestIsComplete_FALSE_WithEpsilonTransition)
    {
        automaton.addTransition(0, fa::Epsilon, 1);
        EXPECT_FALSE(automaton.isComplete());
    }

    TEST_F(AutomatonTest, TestCreateComplete)
    {
        automaton = automaton.createComplete(automaton);
        EXPECT_TRUE(automaton.isComplete());
    }

    TEST_F(AutomatonTest, TestCreateComplement)
    {
        automaton = automaton.createComplement(automaton);
        EXPECT_TRUE(automaton.isComplete());
    }

    // Test pour createMirror()
    TEST_F(AutomatonTest, TestCreateMirror)
    {
        automaton = automaton.createMirror(automaton);
        // Assurez-vous de vérifier si l'automate inversé est correct ici.
    }

    // Test pour makeTransition()
    TEST_F(AutomatonTest, TestMakeTransition)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(2);
        automaton.addTransition(1, 'a', 2);

        // Créez un ensemble d'origine pour représenter l'état de départ
        std::set<int> originState;
        originState.insert(1);

        // Appelez la fonction makeTransition avec l'ensemble d'origine et le symbole 'a'
        std::set<int> resultState = automaton.makeTransition(originState, 'a');

        // Assurez-vous que le résultat contient l'état de destination attendu (2)
        EXPECT_TRUE(resultState.count(2) > 0);
    }

    // Test pour readString()
    TEST_F(AutomatonTest, TestReadString)
    {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(2);
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
        automaton.addState(2);
        automaton.addTransition(1, 'a', 2);

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

} // namespace fa

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
