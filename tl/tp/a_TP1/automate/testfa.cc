
#include "gtest/gtest.h"

#include "Automaton.h"

TEST(AutomatonTest_TP1, AddSymbol) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addSymbol('a'));
  EXPECT_TRUE(fa.addSymbol('b'));
  EXPECT_TRUE(fa.addSymbol('c'));
  EXPECT_FALSE(fa.addSymbol('a'));
  EXPECT_FALSE(fa.addSymbol('b'));
  EXPECT_FALSE(fa.addSymbol('c'));
  EXPECT_FALSE(fa.addSymbol(fa::Epsilon));

}

TEST (AutomatonTest_TP1, RemoveSymbol) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addSymbol('a'));
  EXPECT_TRUE(fa.addSymbol('b'));
  EXPECT_TRUE(fa.addSymbol('c'));
  EXPECT_TRUE(fa.removeSymbol('a'));
  EXPECT_TRUE(fa.removeSymbol('b'));
  EXPECT_FALSE(fa.removeSymbol('b'));
  EXPECT_FALSE(fa.removeSymbol(fa::Epsilon));
}

TEST (AutomatonTest_TP1, HasSymbol_SUCCESS) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addSymbol('a'));
  EXPECT_TRUE(fa.addSymbol('b'));
  EXPECT_TRUE(fa.addSymbol('c'));
  EXPECT_TRUE(fa.hasSymbol('a'));
  EXPECT_TRUE(fa.hasSymbol('b'));
  EXPECT_TRUE(fa.hasSymbol('c'));
}

TEST (AutomatonTest_TP1, HasSymbol_FAIL) {
  fa::Automaton fa;
  EXPECT_FALSE(fa.hasSymbol('a'));
  EXPECT_FALSE(fa.hasSymbol('b'));
  EXPECT_FALSE(fa.hasSymbol('c'));
}

TEST (AutomatonTest_TP1, CountSymbols_SUCCESS) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addSymbol('a'));
  EXPECT_TRUE(fa.addSymbol('b'));
  EXPECT_TRUE(fa.addSymbol('c'));
  EXPECT_EQ(fa.countSymbols(), 3);
}

TEST (AutomatonTest_TP1, AddState_SUCCESS) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(0));
  EXPECT_TRUE(fa.addState(1));
  EXPECT_TRUE(fa.addState(2));
}

TEST (AutomatonTest_TP1, AddState_FAIL) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(0));
  EXPECT_TRUE(fa.addState(1));
  EXPECT_TRUE(fa.addState(2));
  EXPECT_FALSE(fa.addState(0));
  EXPECT_FALSE(fa.addState(1));
  EXPECT_FALSE(fa.addState(2));
}

TEST (AutomatonTest_TP1, RemoveState_SUCCESS) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(0));
  EXPECT_TRUE(fa.addState(1));
  EXPECT_TRUE(fa.removeState(0));
  EXPECT_TRUE(fa.removeState(1));
  EXPECT_FALSE(fa.removeState(1));
}


TEST (AutomatonTest_TP1, HasState_SUCCESS) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(0));
  EXPECT_TRUE(fa.addState(1));
  EXPECT_TRUE(fa.hasState(0));
  EXPECT_TRUE(fa.hasState(1));
  EXPECT_FALSE(fa.hasState(2));
}


TEST (AutomatonTest_TP1, CountStates) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(0));
  EXPECT_TRUE(fa.addState(1));
  EXPECT_TRUE(fa.addState(2));
  EXPECT_EQ(fa.countStates(), 3);
}

TEST (AutomatonTest_TP1, SetStateInitial_SUCCESS) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(0));
  fa.setStateInitial(0);
  EXPECT_TRUE(fa.isStateInitial(0));
}

TEST (AutomatonTest_TP1, IsStateInitial) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(0));
  EXPECT_TRUE(fa.addState(1));
  EXPECT_TRUE(fa.addState(2));
  fa.setStateInitial(0);
  fa.setStateInitial(2);
  EXPECT_TRUE(fa.isStateInitial(0));
  EXPECT_FALSE(fa.isStateInitial(1));
  EXPECT_TRUE(fa.isStateInitial(2));
}



int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
