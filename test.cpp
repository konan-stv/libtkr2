#include <string>
#include "libtkr2.h"
#include <gtest/gtest.h>

int xx = -1;

class someClass : public tkrBaseClass {
private:
    int x;
    void handleEvent(void* data) override {if (data) x = *((int*)(data));}
    void handleClearInt(void* data) {x = xx--;}
public:
    explicit someClass(int a);
    void prn() {std::cout << x << std::endl;}
    int get() {return x;}
};

someClass::someClass(int a) : x (a) {
    regEvent("NEWINT");
    regEvent("CLEARINT", &someClass::handleClearInt);
}

class someClass2 : public tkrBaseClass {
private:
    int x;
    void handleEvent(void* data) override {if (event::is("NEWINT")) return; if (data) x = *((int*)(data));}
    void handleClearInt(void* data) {x = xx--;}
public:
    explicit someClass2(int a);
    void prn() {std::cout << x << std::endl;}
    int get() {return x;}
};

someClass2::someClass2(int a) : x (a) {
    regEvent("NEWINT");
    regEvent("CLEARINT", &someClass2::handleClearInt);
}

someClass *c1, *c2;
someClass2 c3(3);

TEST(testEvents, TESTscCreate) {
    c1 = new someClass(1);
    c2 = new someClass(2);

    EXPECT_EQ(c1->get(), 1);
    EXPECT_EQ(c2->get(), 2);
    EXPECT_EQ(c3.get(), 3);
}

TEST(testEvents, TESTevEvoke) {
    int a = -1;

    event::evoke("NEWINT", &a);
    EXPECT_EQ(c1->get(), -1);
    EXPECT_EQ(c2->get(), -1);
    EXPECT_EQ(c3.get(), 3);

    event::evoke("CLEARINT", &a);
    EXPECT_EQ(c1->get(), -2);
    EXPECT_EQ(c2->get(), -3);
    EXPECT_EQ(c3.get(), -1);
    event::evoke("CLEARINT");
    EXPECT_EQ(c1->get(), -5);
    EXPECT_EQ(c2->get(), -6);
    EXPECT_EQ(c3.get(), -4);

    event::evoke("THTHDHTRH");
    EXPECT_EQ(c1->get(), -5);
    EXPECT_EQ(c2->get(), -6);
    EXPECT_EQ(c3.get(), -4);
}

TEST(testBitmap, bitmap) {
    ifBitmap<uint16_t> b(0xf0);         // создаём и инициализируем экземпляр

    EXPECT_EQ(b.b[2], 0);
    EXPECT_EQ(b.b[4], 1);
    b.clear(4);                         // сброс бита
    EXPECT_EQ(b.b[4], 0);
    uint16_t a = b + 1;                 // присваивание значения
    EXPECT_EQ(a, 225);
    uint16_t& c(b);                     // создаём и иницализируем ссылку на значение
    c = 0xff;
    EXPECT_EQ(b, 255);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

