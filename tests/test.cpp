// Copyright 2018 Your Name <your_email>
#include <gtest/gtest.h>

#include <Transaction.h>
#include <Account.h>

TEST(Transaction, fee_test) {
    Account Sara(228, 201);
    Account David(1337, 0);

    Transaction transaction;
    EXPECT_TRUE(transaction.Make(Sara, David, 150));
    Account Ivan(111, 400);
    EXPECT_FALSE(transaction.Make(Ivan, David, 500));

    EXPECT_THROW(transaction.Make(Ivan, David, 10), std::logic_error);
    Ivan.Lock();
    EXPECT_THROW(transaction.Make(Ivan, David, 100), std::runtime_error);
    Ivan.Unlock();
    EXPECT_TRUE(transaction.Make(Ivan, David, 100));
}

TEST(Transaction, id_is_equal) {
    Account Ivan(228, 1);
    Account Peter(228, 1);

    Transaction transaction;
    try {
        transaction.Make(Ivan, Peter, 100);
        FAIL() << "expected std::logic_error";
    } catch (std::logic_error& err){
        EXPECT_EQ(err.what(), std::string("invalid action"));
    } catch(...) {
        FAIL() << "expected std::logic_error";
    }
}

TEST(Transaction, negative_sum) {
    Account Ivan(228, 123);
    Account Peter(123, 0);

    Transaction transaction;
    try {
        transaction.Make(Ivan, Peter, -12);
        FAIL() << "expected std::invalid argument";
    } catch(std::invalid_argument& err) {
        EXPECT_EQ(err.what(), std::string("sum can't be negative"));
    } catch(...) {
        FAIL() << "expected std::invalid argument";
    }
}

TEST(Transaction, low_sum) {
    Account Ivan(228, 1200);
    Account Peter(123, 0);

    Transaction transaction;
    try {
        transaction.Make(Ivan, Peter, 99);
        FAIL() << "expected std::logic_error";
    } catch(std::logic_error& err) {
        EXPECT_EQ(err.what(), std::string("too small"));
    } catch(...) {
        FAIL() << "expected std::logic error";
    }
}

TEST(Transaction, too_big_fee) {
    Account Ivan(128, 400);
    Account Peter(200, 0);

    Transaction transaction;
    transaction.set_fee(101);

    EXPECT_FALSE(transaction.Make(Ivan, Peter, 200));
}

TEST(Account, account_tests) {
    Account Ivan(100, 1000);

    EXPECT_EQ(1000, Ivan.GetBalance());

    Ivan.Lock();
    Ivan.ChangeBalance(100);
    EXPECT_THROW(Ivan.Lock(), std::runtime_error);
    EXPECT_EQ(Ivan.GetBalance(), 1100);

    Ivan.Unlock();
    EXPECT_THROW(Ivan.ChangeBalance(100), std::runtime_error);
}
