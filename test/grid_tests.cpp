#include <grid/grid.hpp>
#include <gtest/gtest.h>
#include <cstdlib>

template class grid::grid<std::string>;

TEST(grid_tests, grid_constructor_empty)
{
    grid::grid<std::string> gr;
    ASSERT_EQ(gr.width(), 0);
    ASSERT_EQ(gr.height(), 0);
    ASSERT_TRUE(gr.empty());
    ASSERT_FALSE(gr.contains(0,0));
    ASSERT_FALSE(gr.contains(grid::grid_position(0,0)));
}

TEST(grid_tests, grid_constructor_w_h)
{
    grid::grid<std::string> gr(3,2);
    ASSERT_EQ(gr.width(), 3);
    ASSERT_EQ(gr.height(), 2);
    ASSERT_TRUE(!gr.empty());
    ASSERT_TRUE(gr.contains(0,0));
    ASSERT_TRUE(gr.contains(grid::grid_position(0,0)));
    ASSERT_TRUE(gr.contains(2,1));
    ASSERT_FALSE(gr.contains(3,1));
    ASSERT_FALSE(gr.contains(2,2));
}

TEST(grid_tests, grid_constructor_dim)
{
    grid::grid_dimension dim(3,2);
    grid::grid<std::string> gr(dim);
    ASSERT_EQ(gr.width(), 3);
    ASSERT_EQ(gr.height(), 2);
    ASSERT_TRUE(!gr.empty());
    ASSERT_TRUE(gr.contains(0,0));
    ASSERT_TRUE(gr.contains(grid::grid_position(0,0)));
    ASSERT_TRUE(gr.contains(2,1));
    ASSERT_FALSE(gr.contains(3,1));
    ASSERT_FALSE(gr.contains(2,2));
}

TEST(grid_tests, grid_advance)
{
    grid::grid<std::string> gr(3,2);
    grid::grid_position pos(0,0);
    gr.advance(pos);
    ASSERT_EQ(pos, grid::grid_position(1,0));
    pos = grid::grid_position(2,0);
    gr.advance(pos);
    ASSERT_EQ(pos, grid::grid_position(0,1));
    pos = grid::grid_position(2,1);
    gr.advance(pos);
    ASSERT_EQ(pos, grid::grid_position(0,2));
}

TEST(grid_tests, grid_advance_backward)
{
    grid::grid<std::string> gr(3,2);
    grid::grid_position pos(0,0);
    gr.advance_backward(pos);
    ASSERT_EQ(pos, grid::grid_position(2,-1));
    pos = grid::grid_position(0,1);
    gr.advance_backward(pos);
    ASSERT_EQ(pos, grid::grid_position(2,0));
    pos = grid::grid_position(2,1);
    gr.advance_backward(pos);
    ASSERT_EQ(pos, grid::grid_position(1,1));
}

TEST(grid_tests, grid_get_i_j)
{
    grid::grid<std::string> gr(3,2);
    const grid::grid<std::string>& cgr = gr;
    std::string& value = gr.get(0,0);
    ASSERT_EQ(value, "");
    value = "lemon";
    ASSERT_EQ(cgr.get(0,0), "lemon");
    ASSERT_EQ(&cgr.get(0,0), &gr.get(0,0));
}

TEST(grid_tests, grid_get_pos)
{
    grid::grid<std::string> gr(3,2);
    const grid::grid<std::string>& cgr = gr;
    grid::grid_position pos(2,1);
    std::string& value = gr.get(pos);
    ASSERT_EQ(value, "");
    value = "lemon";
    ASSERT_EQ(cgr.get(pos), "lemon");
    ASSERT_EQ(&cgr.get(pos), &gr.get(pos));
}

TEST(grid_tests, grid_get_operator)
{
    grid::grid<std::string> gr(3,2);
    const grid::grid<std::string>& cgr = gr;
    grid::grid_position pos(1,1);
    std::string& value = gr[pos];
    ASSERT_EQ(value, "");
    value = "lemon";
    ASSERT_EQ(cgr[pos], "lemon");
    ASSERT_EQ(&cgr[pos], &gr[pos]);
}

TEST(grid_tests, grid_operators_eq_neq)
{
    grid::grid<std::string> gr(3,2);
    grid::grid<std::string> gr2(3,2);
    ASSERT_EQ(gr, gr2);
    std::vector<std::string> vstr{ "core", "strn", "wgen",
                                   "inis", "cryp", "dirn" };
    auto iter = vstr.begin();
    for (grid::grid_position pos; gr.contains(pos); gr.advance(pos), ++iter)
        gr.get(pos) = *iter;
    ASSERT_NE(gr, gr2);
    iter = vstr.begin();
    for (grid::grid_position pos; gr2.contains(pos); gr2.advance(pos), ++iter)
        gr2.get(pos) = *iter;
    ASSERT_EQ(gr, gr2);
}

TEST(grid_tests, grid_data)
{
    grid::grid<std::string> gr(3,2);
    const grid::grid<std::string>& cgr = gr;
    ASSERT_EQ(gr.data(), &gr.get(0,0));
    ASSERT_EQ(cgr.data(), &cgr.get(0,0));
    ASSERT_EQ(gr.data() + 5, &gr.get(2,1));
    ASSERT_EQ(cgr.data() + 5, &cgr.get(2,1));
}

TEST(grid_tests, grid_clear)
{
    grid::grid<std::string> gr(3,2);
    gr.clear();
    ASSERT_EQ(gr.width(), 0);
    ASSERT_EQ(gr.height(), 0);
    ASSERT_TRUE(gr.empty());
    ASSERT_FALSE(gr.contains(0,0));
    ASSERT_FALSE(gr.contains(grid::grid_position(0,0)));
    ASSERT_EQ(gr, grid::grid<std::string>());
}

TEST(grid_tests, grid_resize_w_h)
{
    grid::grid<std::string> gr(3,2);
    std::vector<std::string> vstr{ "core", "strn", "wgen",
                                   "inis", "cryp", "dirn" };
    auto iter = vstr.begin();
    for (grid::grid_position pos; gr.contains(pos); gr.advance(pos), ++iter)
        gr.get(pos) = *iter;
    ASSERT_EQ(gr.get(0,0), "core");
    ASSERT_EQ(gr.get(1,0), "strn");
    ASSERT_EQ(gr.get(2,0), "wgen");
    ASSERT_EQ(gr.get(0,1), "inis");
    ASSERT_EQ(gr.get(1,1), "cryp");
    ASSERT_EQ(gr.get(2,1), "dirn");
    gr.resize(3,4, "----");
    ASSERT_EQ(gr.width(), 3);
    ASSERT_EQ(gr.height(), 4);
    ASSERT_TRUE(!gr.empty());
    ASSERT_TRUE(gr.contains(2,2));
    ASSERT_TRUE(gr.contains(2,3));
    ASSERT_EQ(gr.get(0,0), "core");
    ASSERT_EQ(gr.get(1,0), "strn");
    ASSERT_EQ(gr.get(2,0), "wgen");
    ASSERT_EQ(gr.get(0,1), "inis");
    ASSERT_EQ(gr.get(1,1), "cryp");
    ASSERT_EQ(gr.get(2,1), "dirn");
    ASSERT_EQ(gr.get(0,2), "----");
    ASSERT_EQ(gr.get(1,2), "----");
    ASSERT_EQ(gr.get(2,2), "----");
    ASSERT_EQ(gr.get(0,3), "----");
    ASSERT_EQ(gr.get(1,3), "----");
    ASSERT_EQ(gr.get(2,3), "----");
    gr.resize(4,3, "****");
    ASSERT_EQ(gr.width(), 4);
    ASSERT_EQ(gr.height(), 3);
    ASSERT_TRUE(!gr.empty());
    ASSERT_TRUE(!gr.contains(2,3));
    ASSERT_TRUE(gr.contains(3,2));
    ASSERT_EQ(gr.get(0,0), "core");
    ASSERT_EQ(gr.get(1,0), "strn");
    ASSERT_EQ(gr.get(2,0), "wgen");
    ASSERT_EQ(gr.get(0,1), "inis");
    ASSERT_EQ(gr.get(1,1), "cryp");
    ASSERT_EQ(gr.get(2,1), "dirn");
    ASSERT_EQ(gr.get(0,2), "----");
    ASSERT_EQ(gr.get(1,2), "----");
    ASSERT_EQ(gr.get(2,2), "----");
    ASSERT_EQ(gr.get(3,0), "****");
    ASSERT_EQ(gr.get(3,1), "****");
    ASSERT_EQ(gr.get(3,2), "****");
    gr.resize(2,2);
    ASSERT_EQ(gr.width(), 2);
    ASSERT_EQ(gr.height(), 2);
    ASSERT_TRUE(!gr.empty());
    ASSERT_TRUE(!gr.contains(3,2));
    ASSERT_TRUE(!gr.contains(2,2));
    ASSERT_TRUE(gr.contains(1,1));
    ASSERT_EQ(gr.get(0,0), "core");
    ASSERT_EQ(gr.get(1,0), "strn");
    ASSERT_EQ(gr.get(0,1), "inis");
    ASSERT_EQ(gr.get(1,1), "cryp");
}

TEST(grid_tests, grid_swap)
{
    grid::grid<std::string> gr(3,2);
    grid::grid<std::string> copy(gr);
    grid::grid<std::string> other;
    std::string* data_ptr = gr.data();
    gr.swap(other);
    ASSERT_NE(gr, other);
    ASSERT_EQ(other, copy);
    ASSERT_EQ(gr, grid::grid<std::string>());
    ASSERT_EQ(data_ptr, other.data());
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}