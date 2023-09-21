#include "Automaton.h"
#include <gtest/gtest.h>

namespace fa {

class AutomatonTest : public ::testing::Test {
protected:
    Automaton automaton;

    virtual void SetUp() {
        automaton.addSymbol('a');
        automaton.addSymbol('b');
        automaton.addState(0);
        automaton.addState(1);
    }
};

TEST_F(AutomatonTest, TestAddSymbol) {
    EXPECT_TRUE(automaton.hasSymbol('a'));
    EXPECT_TRUE(automaton.hasSymbol('b'));
    EXPECT_FALSE(automaton.hasSymbol('c'));
}

TEST_F(AutomatonTest, TestAddState) {
    EXPECT_TRUE(automaton.hasState(0));
    EXPECT_TRUE(automaton.hasState(1));
    EXPECT_FALSE(automaton.hasState(2));
}

TEST_F(AutomatonTest, TestSetStateInitial) {
    automaton.setStateInitial(0);
    EXPECT_TRUE(automaton.isStateInitial(0));
    EXPECT_FALSE(automaton.isStateInitial(1));
}

TEST_F(AutomatonTest, TestSetStateFinal) {
    automaton.setStateFinal(1);
    EXPECT_FALSE(automaton.isStateFinal(0));
    EXPECT_TRUE(automaton.isStateFinal(1));
}

TEST_F(AutomatonTest, TestAddTransition) {
    automaton.addTransition(0, 'a', 1);
    EXPECT_TRUE(automaton.hasTransition(0, 'a', 1));
    EXPECT_FALSE(automaton.hasTransition(1, 'a', 0));
}

TEST_F(AutomatonTest, TestRemoveTransition) {
    automaton.addTransition(0, 'a', 1);
    automaton.removeTransition(0, 'a', 1);
    EXPECT_FALSE(automaton.hasTransition(0, 'a', 1));
}

TEST_F(AutomatonTest, TestCountTransitions) {
    automaton.addTransition(0, 'a', 1);
    automaton.addTransition(1, 'b', 0);
    std::size_t expectedCount = 2;
    EXPECT_EQ(expectedCount, automaton.countTransitions());
}

TEST_F(AutomatonTest, TestEmptyAutomaton) {
    Automaton emptyAutomaton;
    std::ostringstream os;
    emptyAutomaton.prettyPrint(os);
    std::string expectedOutput = 
        "Automaton:\n"
        "Alphabet: { }\n"
        "States: { }\n"
        "Initial States: { }\n"
        "Final States: { }\n"
        "Transitions:\n";
    EXPECT_EQ(expectedOutput, os.str());
}

TEST_F(AutomatonTest, TestBasicAutomaton) {
    automaton.setStateInitial(0);
    automaton.setStateFinal(1);
    automaton.addTransition(0, 'a', 1);
    std::ostringstream os;
    automaton.prettyPrint(os);
    std::string expectedOutput = 
        "Automaton:\n"
        "Alphabet: { a b }\n"
        "States: { 0 1 }\n"
        "Initial States: { 0 }\n"
        "Final States: { 1 }\n"
        "Transitions:\n"
        "  0 --a-> 1\n";
    EXPECT_EQ(expectedOutput, os.str());
}

}  // namespace fa

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
