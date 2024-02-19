#include <CrumpleTree.hpp>
#include <catch2/catch_amalgamated.hpp>

namespace {
namespace proj4 = shindler::ics46::project4;

/* Remember, these are prerequisites to having us grade
    the related part of the assignment.  These are not
    worth points on their own, nor are they comprehensive.

    REMEMBER TO TEST YOUR OWN CODE COMPREHENSIVELY.

*/

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,
// readability-magic-numbers)
TEST_CASE("Tree:SimpleInsertAndFind:ExpectSingleInsertFound",
          "[Required][Basic][Insert][Contains]") {
    proj4::CrumpleTree<int, std::string> tree;
    tree.insert(5, "foo");

    REQUIRE(tree.contains(5));
}

TEST_CASE("Tree:DoubleInsert:ExpectFindSecondInsert",
          "[Required][Basic][Insert][Contains]") {
    proj4::CrumpleTree<int, std::string> tree;
    tree.insert(5, "foo");
    tree.insert(10, "bar");

    REQUIRE(tree.contains(10));
}

TEST_CASE("Tree:QuadInsert:ExpectFindFindTwoHopsAway",
          "[Required][Basic][Insert][Contains]") {
    proj4::CrumpleTree<int, std::string> tree;
    tree.insert(5, "foo");
    tree.insert(3, "sna");
    tree.insert(10, "bar");
    tree.insert(12, "twelve");

    REQUIRE(tree.contains(12));
}

TEST_CASE("Tree:5Inserts:ExpectSize5", "[Required][Basic][Insert][Size]") {
    proj4::CrumpleTree<int, std::string> tree;
    tree.insert(5, "foo");
    tree.insert(3, "sna");
    tree.insert(10, "bar");
    tree.insert(12, "twelve");
    tree.insert(15, "fifteen");

    REQUIRE(tree.size() == 5);
}

TEST_CASE("Tree:DoAPostOrder:ExpectCorrectPostorderTraversal",
          "[Required][Basic][Insert][PostOrder]") {
    proj4::CrumpleTree<int, std::string> tree;
    tree.insert(5, "foo");
    tree.insert(3, "sna");
    tree.insert(10, "bar");
    auto trav = tree.postOrder();
    static const std::vector<int> expected = {3, 10, 5};
    REQUIRE(trav == expected);
}

TEST_CASE("Tree:RemoveDoesARemove:ExpectElementsNotInTreeAndLevelsCorrect",
          "[Required][Insert][Erase][Contains][Level]") {
    proj4::CrumpleTree<int, std::string> tree;
    tree.insert(20, "are");
    tree.insert(19, "you");
    tree.insert(18, "following");
    tree.insert(12, "from");
    tree.insert(9, "lecture?");
    REQUIRE(tree.contains(20));
    REQUIRE(tree.contains(19));
    REQUIRE(tree.contains(18));
    REQUIRE(tree.contains(12));
    REQUIRE(tree.contains(9));
    tree.remove(9);
    tree.remove(18);
    // What happens?  12 is now a leaf and must fall.  But 19 is a (2,2) and
    // remains at level 3.
    REQUIRE(!tree.contains(9));
    REQUIRE(!tree.contains(18));
    REQUIRE(tree.level(20) == 1);
    REQUIRE(tree.level(12) == 1);
    REQUIRE(tree.level(19) == 3);
}

//steptest
TEST_CASE("insert_condition3", "[Required][Basic][Insert][Size]") {
    proj4::CrumpleTree<int, std::string> tree;
    tree.insert(5, "foo");
    tree.insert(3, "sna");
    tree.insert(10, "bar");
    tree.insert(12, "twelve");
    tree.insert(15, "fifteen");
    tree.insert(2, "two");
    tree.insert(1, "one");

    REQUIRE(tree.size() == 7);
}

TEST_CASE("Condition3_2:ExpectElementsNotInTreeAndLevelsCorrect",
          "[Required][Insert][Erase][Contains][Level]") {
    proj4::CrumpleTree<int, std::string> tree;
    tree.insert(20, "are");
    tree.insert(19, "you");
    tree.insert(18, "following");
    tree.insert(12, "from");
    tree.insert(9, "lecture?");
    REQUIRE(tree.size() == 5);
}


TEST_CASE("Condition5_1:ExpectElementsNotInTreeAndLevelsCorrect",
          "[Required][Insert][Erase][Contains][Level]") {
    proj4::CrumpleTree<int, std::string> tree;
    tree.insert(20, "are");
    tree.insert(19, "you");
    tree.insert(18, "following");
    tree.insert(12, "from");
    tree.insert(17, "lecture?");
    REQUIRE(tree.size() == 5);
    REQUIRE(tree.level(17) == 2);
    REQUIRE(tree.level(12) == 1);
    REQUIRE(tree.level(18) == 1);
    REQUIRE(tree.level(19) == 3);
}

TEST_CASE("Condition5_2:ExpectElementsNotInTreeAndLevelsCorrect",
          "[Required][Insert][Erase][Contains][Level]") {
    proj4::CrumpleTree<int, std::string> tree;
    tree.insert(20, "20");
    tree.insert(19, "19");
    tree.insert(18, "18");
    tree.insert(12, "12");
    tree.insert(9, "9");
    tree.insert(17, "17");
    tree.insert(7, "7");
    tree.insert(5, "5");
    tree.insert(13, "13");
    tree.insert(15, "15");
    tree.insert(6, "6");
    tree.insert(16, "16");
    tree.insert(14, "14");

    REQUIRE(tree.size() == 13);
    REQUIRE(tree.level(12) == 5);
    REQUIRE(tree.level(18) == 4);
    REQUIRE(tree.level(7) == 3);
    REQUIRE(tree.level(15) == 3);
    REQUIRE(tree.level(5) == 2);
    REQUIRE(tree.level(13) == 2);
    REQUIRE(tree.level(17) == 2);
    REQUIRE(tree.level(19) == 2);
    REQUIRE(tree.level(6) == 1);
    REQUIRE(tree.level(9) == 1);
    REQUIRE(tree.level(14) == 1);
    REQUIRE(tree.level(16) == 1);
    REQUIRE(tree.level(20) == 1);
}
// NOLINTEND

}  // namespace
