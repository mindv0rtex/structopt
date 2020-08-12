#include <doctest.hpp>
#include <structopt/structopt.hpp>

using doctest::test_suite;

struct Command {  
  struct SubCommand1 {
    int bar{0};
    double value{0.0};
    std::optional<bool> verbose;
  };
  SubCommand1 foo;

  struct SubCommand2 {
    std::optional<bool> global;
  };
  SubCommand2 config;
};
STRUCTOPT(Command::SubCommand1, bar, value, verbose);
STRUCTOPT(Command::SubCommand2, global);
STRUCTOPT(Command, foo, config);

TEST_CASE("structopt can parse multiple nested struct arguments" * test_suite("nested_struct")) {
  {
    auto arguments = structopt::parse<Command>(std::vector<std::string>{"./main", "foo", "15", "3.14", "--verbose", "true"});
    REQUIRE(arguments.foo.bar == 15);
    REQUIRE(arguments.foo.value == 3.14);
    REQUIRE(arguments.foo.verbose.value() == true);
    REQUIRE(not arguments.config.global.has_value());
  }
  {
    auto arguments = structopt::parse<Command>(std::vector<std::string>{"./main", "foo", "-v", "true", "15", "3.14"});
    REQUIRE(arguments.foo.bar == 15);
    REQUIRE(arguments.foo.value == 3.14);
    REQUIRE(arguments.foo.verbose.value() == true);
    REQUIRE(not arguments.config.global.has_value());
  }
  {
    auto arguments = structopt::parse<Command>(std::vector<std::string>{"./main", "config", "--global", "true"});
    REQUIRE(arguments.foo.bar == 0);
    REQUIRE(arguments.foo.value == 0.0);
    REQUIRE(not arguments.foo.verbose.has_value());
    REQUIRE(arguments.config.global == true);
  }
  {
    auto arguments = structopt::parse<Command>(std::vector<std::string>{"./main", "config", "-g", "false"});
    REQUIRE(arguments.foo.bar == 0);
    REQUIRE(arguments.foo.value == 0.0);
    REQUIRE(not arguments.foo.verbose.has_value());
    REQUIRE(arguments.config.global == false);
  }
}