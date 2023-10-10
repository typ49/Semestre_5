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

    // Test TP1 *******************************************************************************************************

    TEST_F(AutomatonTest, TestIsValid_TRUE)
    {
        EXPECT_TRUE(automaton.isValid());
    }

    TEST_F(AutomatonTest, TestIsValid_FALSE)
    {
        EXPECT_FALSE(notAnAutomaton.isValid());
    }

    TEST_F(AutomatonTest, TestHasSymbol_TRUE)
    {
        EXPECT_TRUE(automaton.hasSymbol('a'));
    }

    TEST_F(AutomatonTest, TestHasSymbol_FALSE)
    {
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
        EXPECT_FALSE(automaton.addSymbol('a'));
    }

    TEST_F(AutomatonTest, TestRemoveSymbol_TRUE)
    {
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
        std::size_t expectedCount = 2;
        EXPECT_EQ(expectedCount, automaton.countSymbols());
    }

    TEST_F(AutomatonTest, TestHasState_TRUE)
    {
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
        EXPECT_FALSE(automaton.addState(0));
    }

    TEST_F(AutomatonTest, TestRemoveState_TRUE)
    {
        EXPECT_TRUE(automaton.removeState(0));
    }

    TEST_F(AutomatonTest, TestRemoveState_FALSE_NotPresent)
    {
        EXPECT_FALSE(automaton.removeState(2));
    }

    TEST_F(AutomatonTest, TestCountStates)
    {
        std::size_t expectedCount = 2;
        EXPECT_EQ(expectedCount, automaton.countStates());
    }

    TEST_F(AutomatonTest, TestSetAndGetStateInitial)
    {
        automaton.setStateInitial(1);
        EXPECT_TRUE(automaton.isStateInitial(1));
        EXPECT_TRUE(automaton.isStateInitial(0));
    }

    TEST_F(AutomatonTest, TestSetAndGetStateFinal)
    {
        automaton.setStateFinal(0);
        EXPECT_TRUE(automaton.isStateFinal(0));
        EXPECT_TRUE(automaton.isStateFinal(1));
    }

    TEST_F(AutomatonTest, TestAddTransition_TRUE)
    {
        EXPECT_TRUE(automaton.addTransition(0, 'b', 1));
    }

    TEST_F(AutomatonTest, TestAddTransition_FALSE_AlreadyPresent)
    {
        EXPECT_FALSE(automaton.addTransition(0, 'a', 1));
    }

    TEST_F(AutomatonTest, TestAddTransition_FALSE_StateNotPresent)
    {
        EXPECT_FALSE(automaton.addTransition(0, 'a', 2));
    }

    TEST_F(AutomatonTest, TestAddTransition_FALSE_SymbolNotPresent)
    {
        EXPECT_FALSE(automaton.addTransition(0, 'c', 1));
    }

    TEST_F(AutomatonTest, TestRemoveTransition_TRUE)
    {
        EXPECT_TRUE(automaton.removeTransition(0, 'a', 1));
    }

    TEST_F(AutomatonTest, TestRemoveTransition_FALSE_NotPresent)
    {
        EXPECT_FALSE(automaton.removeTransition(0, 'b', 1));
    }

    TEST_F(AutomatonTest, TestCountTransitions)
    {
        automaton.addTransition(1, 'b', 0);
        std::size_t expectedCount = 2;
        EXPECT_EQ(expectedCount, automaton.countTransitions());
    }

    TEST_F(AutomatonTest, TestHasTransition_TRUE)
    {
        EXPECT_TRUE(automaton.hasTransition(0, 'a', 1));
    }

    TEST_F(AutomatonTest, TestHasTransition_FALSE_NotPresent)
    {
        EXPECT_FALSE(automaton.hasTransition(0, 'b', 1));
    }

    TEST_F(AutomatonTest, TestHasTransition_FALSE_StateNotPresent)
    {
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
        forPrint.prettyPrint(std::cout);
    }

    // Test TP2 *******************************************************************************************************

    TEST_F(AutomatonTest, TestHasEpsilonTransition_TRUE_usingHasTransition)
    {
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

    // TODO: Add tests for createMirror(), makeTransition(), readString(), and match() methods

} // namespace fa

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}