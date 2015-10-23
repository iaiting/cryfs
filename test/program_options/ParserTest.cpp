#include "testutils/ProgramOptionsTestBase.h"
#include "../../src/program_options/Parser.h"

using namespace cryfs::program_options;
using std::vector;

class ProgramOptionsParserTest: public ProgramOptionsTestBase {
public:
    ProgramOptions parse(std::initializer_list<const char*> options) {
        vector<char*> _options = ProgramOptionsTestBase::options(options);
        return Parser(_options.size(), _options.data()).parse();
    }
};

class ProgramOptionsParserDeathTest: public ProgramOptionsParserTest {};

TEST_F(ProgramOptionsParserDeathTest, MissingAllOptions) {
    EXPECT_DEATH(
        parse({"./myExecutable"}),
        "Usage:"
    );
}

TEST_F(ProgramOptionsParserDeathTest, MissingDir) {
    EXPECT_DEATH(
        parse({"./myExecutable", "/home/user/baseDir"}),
        "Usage:"
    );
}

TEST_F(ProgramOptionsParserDeathTest, ConfigFileMissing) {
    EXPECT_DEATH(
            parse({"./myExecutable", "/home/user/baseDir", "/home/user/mountDir"}),
    "Usage:"
    );
}

TEST_F(ProgramOptionsParserDeathTest, HelpLongOption) {
    EXPECT_DEATH(
            parse({"./myExecutable", "--help"}),
    "Usage:"
    );
}

TEST_F(ProgramOptionsParserDeathTest, HelpShortOption) {
    EXPECT_DEATH(
            parse({"./myExecutable", "-h"}),
    "Usage:"
    );
}

TEST_F(ProgramOptionsParserTest, NoSpecialOptions) {
    ProgramOptions options = parse({"./myExecutable", "--config", "/home/user/configFile", "/home/user/baseDir", "/home/user/mountDir"});
    EXPECT_EQ("/home/user/baseDir", options.baseDir());
    EXPECT_EQ("/home/user/mountDir", options.mountDir());
    EXPECT_VECTOR_EQ({"./myExecutable", "/home/user/mountDir"}, options.fuseOptions());
}

TEST_F(ProgramOptionsParserTest, FuseOptionGiven) {
    ProgramOptions options = parse({"./myExecutable", "--config", "/home/user/configFile", "/home/user/baseDir", "/home/user/mountDir", "--", "-f"});
    EXPECT_EQ("/home/user/baseDir", options.baseDir());
    EXPECT_EQ("/home/user/mountDir", options.mountDir());
    EXPECT_VECTOR_EQ({"./myExecutable", "/home/user/mountDir", "-f"}, options.fuseOptions());
}