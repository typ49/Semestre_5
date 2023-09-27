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

    TEST_F(AutomatonTest, TestIsValid)
    {
        EXPECT_TRUE(automaton.isValid());
        EXPECT_FALSE(notAnAutomaton.isValid());
    }

    TEST_F(AutomatonTest, TestHasSymbol)
    {
        EXPECT_TRUE(automaton.hasSymbol('a'));
        EXPECT_FALSE(automaton.hasSymbol('c'));
    }

    TEST_F(AutomatonTest, TestAddSymbol)
    {
        EXPECT_FALSE(automaton.addSymbol('\0')); // caractere non valide
        EXPECT_FALSE(automaton.addSymbol('a'));  // caractere deja present
        EXPECT_TRUE(automaton.hasSymbol('a'));
        EXPECT_TRUE(automaton.hasSymbol('b'));
        EXPECT_FALSE(automaton.hasSymbol('c'));
    }

    TEST_F(AutomatonTest, TestRemoveSymbol)
    {
        EXPECT_TRUE(automaton.removeSymbol('a'));  // caractere present
        EXPECT_FALSE(automaton.removeSymbol('a')); // caractere deja supprime
        EXPECT_FALSE(automaton.removeSymbol('c')); // caractere non present
        EXPECT_FALSE(automaton.hasSymbol('a'));
    }

    TEST_F(AutomatonTest, TestCountSymbols)
    {
        std::size_t expectedCount = 2;
        EXPECT_EQ(expectedCount, automaton.countSymbols());
    }

    TEST_F(AutomatonTest, TestHasState)
    {
        EXPECT_TRUE(automaton.hasState(0));
        EXPECT_FALSE(automaton.hasState(2));
    }

    TEST_F(AutomatonTest, TestAddState)
    {
        EXPECT_FALSE(automaton.addState(0)); // etat deja present
        EXPECT_TRUE(automaton.hasState(0));
        EXPECT_TRUE(automaton.hasState(1));
        EXPECT_FALSE(automaton.hasState(2));
    }

    TEST_F(AutomatonTest, TestRemoveState)
    {
        EXPECT_TRUE(automaton.removeState(0));  // etat present
        EXPECT_FALSE(automaton.removeState(0)); // etat deja supprime
        EXPECT_FALSE(automaton.hasState(0));
        EXPECT_FALSE(automaton.hasTransition(0, 'a', 1));
        EXPECT_FALSE(automaton.isStateInitial(0));
    }

    TEST_F(AutomatonTest, TestCountStates)
    {
        std::size_t expectedCount = 2;
        EXPECT_EQ(expectedCount, automaton.countStates());
    }

    TEST_F(AutomatonTest, TestSetStateInitial)
    {
        EXPECT_TRUE(automaton.isStateInitial(0));
        EXPECT_FALSE(automaton.isStateInitial(1));
    }

    TEST_F(AutomatonTest, TestSetStateFinal)
    {
        EXPECT_FALSE(automaton.isStateFinal(0));
        EXPECT_TRUE(automaton.isStateFinal(1));
    }

    TEST_F(AutomatonTest, TestAddTransition)
    {
        EXPECT_TRUE(automaton.addTransition(0, 'b', 1));
        EXPECT_FALSE(automaton.addTransition(0, 'a', 1)); // transition deja presente
        EXPECT_FALSE(automaton.addTransition(0, 'c', 1)); // symbole non present
        EXPECT_FALSE(automaton.addTransition(0, 'a', 2)); // etat non present
        EXPECT_FALSE(automaton.addTransition(2, 'a', 1)); // etat non present
        EXPECT_TRUE(automaton.hasTransition(0, 'a', 1));
        EXPECT_FALSE(automaton.hasTransition(1, 'a', 0));
    }

    TEST_F(AutomatonTest, TestRemoveTransition)
    {
        EXPECT_TRUE(automaton.removeTransition(0, 'a', 1));
        EXPECT_FALSE(automaton.removeTransition(0, 'a', 1)); // transition deja supprimee
        EXPECT_FALSE(automaton.removeTransition(0, 'b', 1)); // transition non presente
        EXPECT_FALSE(automaton.hasTransition(0, 'a', 1));
    }

    TEST_F(AutomatonTest, TestCountTransitions)
    {
        automaton.addTransition(1, 'b', 0);
        std::size_t expectedCount = 2;
        EXPECT_EQ(expectedCount, automaton.countTransitions());
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

} // namespace fa

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
