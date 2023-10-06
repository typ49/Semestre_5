#include "Automaton.h"
#include <gtest/gtest.h>

namespace fa
{

    class AutomatonTest : public ::testing::Test
    {
    protected:
        Automaton automaton;
        Automaton notAnAutomaton;
        Automaton forPrint;

        virtual void SetUp()
        {
            automaton.addSymbol('a');
            automaton.addSymbol('b');
            automaton.addState(0);
            automaton.addState(1);
            automaton.setStateInitial(0);
            automaton.setStateFinal(1);
            automaton.addTransition(0, 'a', 1);
        }
    };

    /**
     * Test TP1 *******************************************************************************************************
     */

    /**
     * Test of isValid() method
     */
    TEST_F(AutomatonTest, TestIsValid_TRUE)
    {
        EXPECT_TRUE(automaton.isValid());
    }

    TEST_F(AutomatonTest, TestIsValid_FALSE)
    {
        EXPECT_FALSE(notAnAutomaton.isValid());
    }

    /**
     * Test of hasSymbol() method
     */
    TEST_F(AutomatonTest, TestHasSymbol_TRUE)
    {
        EXPECT_TRUE(automaton.hasSymbol('a'));
    }

    TEST_F(AutomatonTest, TestHasSymbol_FALSE)
    {
        EXPECT_FALSE(automaton.hasSymbol('c'));
    }

    /**
     * Test of addSymbol() method
     */
    TEST_F(AutomatonTest, TestAddSymbol_TRUE)
    {
        EXPECT_TRUE(automaton.addSymbol('c'));
    }

    TEST_F(AutomatonTest, TestAddSymbol_FALSE)
    {
        EXPECT_FALSE(automaton.addSymbol('\0')); // caractere non valide
        EXPECT_FALSE(automaton.addSymbol('a'));  // caractere deja present
    }

    /**
     * Test of removeSymbol() method
     */
    TEST_F(AutomatonTest, TestRemoveSymbol_TRUE)
    {
        EXPECT_TRUE(automaton.removeSymbol('a')); // caractere present
    }

    TEST_F(AutomatonTest, TestRemoveSymbol_FALSE)
    {
        EXPECT_FALSE(automaton.removeSymbol('\0')); // caractere non valide
        EXPECT_FALSE(automaton.removeSymbol('c'));  // caractere non present
    }

    /**
     * Test of countSymbols() method
     */
    TEST_F(AutomatonTest, TestCountSymbols)
    {
        std::size_t expectedCount = 2;
        EXPECT_EQ(expectedCount, automaton.countSymbols());
    }

    /**
     * Test of hasState() method
     */
    TEST_F(AutomatonTest, TestHasState_TRUE)
    {
        EXPECT_TRUE(automaton.hasState(0));
    }

    TEST_F(AutomatonTest, TestHasState_FALSE)
    {
        EXPECT_FALSE(automaton.hasState(2));
    }

    /**
     * Test of addState() method
     */
    TEST_F(AutomatonTest, TestAddState_TRUE)
    {
        EXPECT_TRUE(automaton.addState(2));
    }

    TEST_F(AutomatonTest, TestAddState_FALSE)
    {
        EXPECT_FALSE(automaton.addState(0)); // etat deja present
    }

    /**
     * Test of removeState() method
     */
    TEST_F(AutomatonTest, TestRemoveState_TRUE)
    {
        EXPECT_TRUE(automaton.removeState(0)); // etat present
    }

    TEST_F(AutomatonTest, TestRemoveState_FALSE)
    {
        EXPECT_FALSE(automaton.removeState(2)); // etat non present
    }

    /**
     * Test of countStates() method
     */
    TEST_F(AutomatonTest, TestCountStates)
    {
        std::size_t expectedCount = 2;
        EXPECT_EQ(expectedCount, automaton.countStates());
    }

    /**
     * Test of setStateInitial() method and isStateInitial() method
     */
    TEST_F(AutomatonTest, TestSetStateInitial_TRUE)
    {
        automaton.setStateInitial(1);
        EXPECT_TRUE(automaton.isStateInitial(1));
    }

    TEST_F(AutomatonTest, TestSetStateInitial_AlreadyInitial)
    {
        automaton.setStateInitial(0);
        EXPECT_TRUE(automaton.isStateInitial(0));
    }

    TEST_F(AutomatonTest, TestSetStateInitial_StateDoesntExist)
    {
        automaton.setStateInitial(2);
        EXPECT_FALSE(automaton.isStateInitial(2));
    }

    TEST_F(AutomatonTest, TestIsStateInitial_TRUE)
    {
        EXPECT_TRUE(automaton.isStateInitial(0));
    }

    TEST_F(AutomatonTest, TestIsStateInitial_FALSE)
    {
        EXPECT_FALSE(automaton.isStateInitial(1));
    }

    /**
     * Test of setStateFinale() method and isStateFinal() method
     */
    TEST_F(AutomatonTest, TestSetStateFinal_TRUE)
    {
        EXPECT_TRUE(automaton.isStateFinal(1));
    }

    TEST_F(AutomatonTest, TestSetStateFinal_FALSE)
    {
        EXPECT_FALSE(automaton.isStateFinal(0));
    }

    TEST_F(AutomatonTest, TestIsStateFinal_TRUE)
    {
        EXPECT_TRUE(automaton.isStateFinal(1));
    }

    TEST_F(AutomatonTest, TestIsStateFinal_FALSE)
    {
        EXPECT_FALSE(automaton.isStateFinal(0));
    }

    /**
     * Test of addTransition() method
     */
    TEST_F(AutomatonTest, TestAddTransition_TRUE)
    {
        EXPECT_TRUE(automaton.addTransition(0, 'b', 1));
    }

    TEST_F(AutomatonTest, TestAddTransition_FALSE)
    {
        EXPECT_FALSE(automaton.addTransition(0, 'a', 1)); // transition deja presente
        EXPECT_FALSE(automaton.addTransition(0, 'a', 2)); // etat non present
        EXPECT_FALSE(automaton.addTransition(0, 'c', 1)); // symbole non present
    }

    /**
     * Test of removeTransition() method
     */

    TEST_F(AutomatonTest, TestRemoveTransition_TRUE)
    {
        EXPECT_TRUE(automaton.removeTransition(0, 'a', 1));
    }

    TEST_F(AutomatonTest, TestRemoveTransition_FALSE)
    {
        EXPECT_FALSE(automaton.removeTransition(0, 'b', 1)); // transition non presente
        EXPECT_FALSE(automaton.removeTransition(0, 'a', 2)); // etat non present
        EXPECT_FALSE(automaton.removeTransition(0, 'c', 1)); // symbole non present
    }

    /**
     * Test of hasTransition() method
     */
    TEST_F(AutomatonTest, TestCountTransitions)
    {
        automaton.addTransition(1, 'b', 0);
        std::size_t expectedCount = 2;
        EXPECT_EQ(expectedCount, automaton.countTransitions());
    }

    /**
     * Test of hasTransition() method
     */
    TEST_F(AutomatonTest, TestHasTransition_TRUE)
    {
        EXPECT_TRUE(automaton.hasTransition(0, 'a', 1));
    }

    TEST_F(AutomatonTest, TestHasTransition_FALSE)
    {
        EXPECT_FALSE(automaton.hasTransition(0, 'b', 1)); // transition non presente
        EXPECT_FALSE(automaton.hasTransition(0, 'a', 2)); // etat non present
        EXPECT_FALSE(automaton.hasTransition(0, 'c', 1)); // symbole non present
    }

    /**
     * Test of PrettyPrint() method
     */
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

        forPrint.prettyPrint(std::cout);
    }

    /**
     * Test TP2 *******************************************************************************************************
     */

    /**
     * Test of hasEpsilonTransition
     */

    TEST_F(AutomatonTest, TestHasEpsilonTransition_TRUE)
    {
        automaton.addTransition(0, fa::Epsilon, 1);
        EXPECT_TRUE(automaton.hasEpsilonTransition());
    }

    TEST_F(AutomatonTest, TestHasEpsilonTransition_FALSE)
    {
        EXPECT_FALSE(automaton.hasEpsilonTransition());
    }

    /**
     * Test of isDeterministic() method
     */

    TEST_F(AutomatonTest, TestIsDeterministic_TRUE)
    {
        EXPECT_TRUE(automaton.isDeterministic());
    }

    TEST_F(AutomatonTest, TestIsDeterministic_FALSE)
    {
        automaton.addTransition(0, 'a', 0);
        EXPECT_FALSE(automaton.isDeterministic());
    }

    TEST_F(AutomatonTest, TestIsDeterministic_NoInitialState)
    {
        automaton.removeState(0);
        EXPECT_FALSE(automaton.isDeterministic());
    }

    TEST_F(AutomatonTest, TestIsDeterministic_MultipleInitialState)
    {
        automaton.setStateInitial(1);
        EXPECT_FALSE(automaton.isDeterministic());
    }

    TEST_F(AutomatonTest, TestIsDeterministic_WithEpsilonTransition)
    {
        automaton.addTransition(0, fa::Epsilon, 1);
        EXPECT_FALSE(automaton.isDeterministic());
    }

    TEST_F(AutomatonTest, TestIsDeterministic_WithMultipleTransition)
    {
        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(0, 'b', 1);
        EXPECT_FALSE(automaton.isDeterministic());
    }

    /**
     * Test of isComplete() method
     */

    TEST_F(AutomatonTest, TestIsComplete_TRUE)
    {
        automaton.addTransition(0, 'b', 1);
        EXPECT_TRUE(automaton.isComplete());
    }

    TEST_F(AutomatonTest, TestIsComplete_FALSE)
    {
        EXPECT_FALSE(automaton.isComplete());
    }

    TEST_F(AutomatonTest, TestIsComplete_NoInitialState)
    {
        automaton.removeState(0);
        EXPECT_FALSE(automaton.isComplete());
    }

    TEST_F(AutomatonTest, TestIsComplete_MultipleInitialState)
    {
        automaton.setStateInitial(1);
        EXPECT_FALSE(automaton.isComplete());
    }

    TEST_F(AutomatonTest, TestIsComplete_WithEpsilonTransition)
    {
        automaton.addTransition(0, fa::Epsilon, 1);
        EXPECT_FALSE(automaton.isComplete());
    }

    TEST_F(AutomatonTest, TestIsComplete_WithMultipleTransition)
    {
        automaton.addTransition(0, 'a', 1);
        automaton.addTransition(0, 'b', 1);
        EXPECT_FALSE(automaton.isComplete());
    }

    /**
     * Test of createComplete() method
     */

    TEST_F(AutomatonTest, TestCreateComplete_NotComplete)
    {
        automaton.addTransition(0, 'b', 1);
        EXPECT_FALSE(automaton.isComplete());
        automaton = automaton.createComplete(automaton);
        EXPECT_TRUE(automaton.isComplete());
    }

    TEST_F(AutomatonTest, TestCreateComplete_Complete)
    {
        EXPECT_TRUE(automaton.isComplete());
        int intialCountTransition = automaton.countTransitions();
        automaton = automaton.createComplete(automaton);
        EXPECT_TRUE(automaton.isComplete());
        EXPECT_EQ(intialCountTransition, automaton.countTransitions());
    }

    /**
     * Test of createComplement() method
     */

    TEST_F(AutomatonTest, TestCreateComplement)
    {
        automaton = automaton.createComplement(automaton);
        EXPECT_TRUE(automaton.isComplete());
    }

    /**
     * Test of createMirror() method
     * TODO
     */

    /**
     * Test of makeTransition() method
     * TODO
     */

    /**
     * Test of readString() method
     * TODO
     */

    /**
     * Test of match() method
     * TODO
     */

} // namespace fa

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
