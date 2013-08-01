#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "util.h"

BOOST_AUTO_TEST_SUITE(getarg_tests)

static void
ResetArgs(const std::string& strArg)
{
    std::vector<std::string> vecArg;
    boost::split(vecArg, strArg, boost::is_space(), boost::token_compress_on);

    // Insert dummy executable name:
    vecArg.insert(vecArg.begin(), "testbitcoin");

    // Convert to char*:
    std::vector<const char*> vecChar;
    BOOST_FOREACH(std::string& s, vecArg)
        vecChar.push_back(s.c_str());

    ParseParameters(vecChar.size(), &vecChar[0]);
}

BOOST_AUTO_TEST_CASE(boolarg)
{
    ResetArgs("-mst");
    BOOST_CHECK(GetBoolArg("-mst"));
    BOOST_CHECK(GetBoolArg("-mst", false));
    BOOST_CHECK(GetBoolArg("-mst", true));

    BOOST_CHECK(!GetBoolArg("-fo"));
    BOOST_CHECK(!GetBoolArg("-fo", false));
    BOOST_CHECK(GetBoolArg("-fo", true));

    BOOST_CHECK(!GetBoolArg("-msto"));
    BOOST_CHECK(!GetBoolArg("-msto", false));
    BOOST_CHECK(GetBoolArg("-msto", true));

    ResetArgs("-mst=0");
    BOOST_CHECK(!GetBoolArg("-mst"));
    BOOST_CHECK(!GetBoolArg("-mst", false));
    BOOST_CHECK(!GetBoolArg("-mst", true));

    ResetArgs("-mst=1");
    BOOST_CHECK(GetBoolArg("-mst"));
    BOOST_CHECK(GetBoolArg("-mst", false));
    BOOST_CHECK(GetBoolArg("-mst", true));

    // New 0.6 feature: auto-map -nosomething to !-something:
    ResetArgs("-nomst");
    BOOST_CHECK(!GetBoolArg("-mst"));
    BOOST_CHECK(!GetBoolArg("-mst", false));
    BOOST_CHECK(!GetBoolArg("-mst", true));

    ResetArgs("-nomst=1");
    BOOST_CHECK(!GetBoolArg("-mst"));
    BOOST_CHECK(!GetBoolArg("-mst", false));
    BOOST_CHECK(!GetBoolArg("-mst", true));

    ResetArgs("-mst -nomst");  // -mst should win
    BOOST_CHECK(GetBoolArg("-mst"));
    BOOST_CHECK(GetBoolArg("-mst", false));
    BOOST_CHECK(GetBoolArg("-mst", true));

    ResetArgs("-mst=1 -nomst=1");  // -mst should win
    BOOST_CHECK(GetBoolArg("-mst"));
    BOOST_CHECK(GetBoolArg("-mst", false));
    BOOST_CHECK(GetBoolArg("-mst", true));

    ResetArgs("-mst=0 -nomst=0");  // -mst should win
    BOOST_CHECK(!GetBoolArg("-mst"));
    BOOST_CHECK(!GetBoolArg("-mst", false));
    BOOST_CHECK(!GetBoolArg("-mst", true));

    // New 0.6 feature: treat -- same as -:
    ResetArgs("--mst=1");
    BOOST_CHECK(GetBoolArg("-mst"));
    BOOST_CHECK(GetBoolArg("-mst", false));
    BOOST_CHECK(GetBoolArg("-mst", true));

    ResetArgs("--nomst=1");
    BOOST_CHECK(!GetBoolArg("-mst"));
    BOOST_CHECK(!GetBoolArg("-mst", false));
    BOOST_CHECK(!GetBoolArg("-mst", true));

}

BOOST_AUTO_TEST_CASE(stringarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-mst", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-mst", "eleven"), "eleven");

    ResetArgs("-mst -bar");
    BOOST_CHECK_EQUAL(GetArg("-mst", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-mst", "eleven"), "");

    ResetArgs("-mst=");
    BOOST_CHECK_EQUAL(GetArg("-mst", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-mst", "eleven"), "");

    ResetArgs("-mst=11");
    BOOST_CHECK_EQUAL(GetArg("-mst", ""), "11");
    BOOST_CHECK_EQUAL(GetArg("-mst", "eleven"), "11");

    ResetArgs("-mst=eleven");
    BOOST_CHECK_EQUAL(GetArg("-mst", ""), "eleven");
    BOOST_CHECK_EQUAL(GetArg("-mst", "eleven"), "eleven");

}

BOOST_AUTO_TEST_CASE(intarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-mst", 11), 11);
    BOOST_CHECK_EQUAL(GetArg("-mst", 0), 0);

    ResetArgs("-mst -bar");
    BOOST_CHECK_EQUAL(GetArg("-mst", 11), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);

    ResetArgs("-mst=11 -bar=12");
    BOOST_CHECK_EQUAL(GetArg("-mst", 0), 11);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 12);

    ResetArgs("-mst=NaN -bar=NotANumber");
    BOOST_CHECK_EQUAL(GetArg("-mst", 1), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);
}

BOOST_AUTO_TEST_CASE(doubledash)
{
    ResetArgs("--mst");
    BOOST_CHECK_EQUAL(GetBoolArg("-mst"), true);

    ResetArgs("--mst=verbose --bar=1");
    BOOST_CHECK_EQUAL(GetArg("-mst", ""), "verbose");
    BOOST_CHECK_EQUAL(GetArg("-bar", 0), 1);
}

BOOST_AUTO_TEST_CASE(boolargno)
{
    ResetArgs("-nomst");
    BOOST_CHECK(!GetBoolArg("-mst"));
    BOOST_CHECK(!GetBoolArg("-mst", true));
    BOOST_CHECK(!GetBoolArg("-mst", false));

    ResetArgs("-nomst=1");
    BOOST_CHECK(!GetBoolArg("-mst"));
    BOOST_CHECK(!GetBoolArg("-mst", true));
    BOOST_CHECK(!GetBoolArg("-mst", false));

    ResetArgs("-nomst=0");
    BOOST_CHECK(GetBoolArg("-mst"));
    BOOST_CHECK(GetBoolArg("-mst", true));
    BOOST_CHECK(GetBoolArg("-mst", false));

    ResetArgs("-mst --nomst");
    BOOST_CHECK(GetBoolArg("-mst"));

    ResetArgs("-nomst -mst"); // mst always wins:
    BOOST_CHECK(GetBoolArg("-mst"));
}

BOOST_AUTO_TEST_SUITE_END()
