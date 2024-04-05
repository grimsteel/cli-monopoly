all: bin/CmdLineMonopoly

CXX = g++
override CXXFLAGS += -g -std=c++20 -MMD
override LDFLAGS += -g -lncursesw -std=c++20
BAT_PARAMS = --theme="mocha" --style header,numbers,grid,snip -f
AHA = aha --black -y "color: \#CDD6F4; background-color: \#1E1E2E; font-size: 12pt;" | sed 's/df8e1d/&30/g'
REMOVE_COMMENTS = sed -E 's$$//[^\n\r]+$$$$'
PROJ_REF_FILTER = $(REMOVE_COMMENTS) | ./textimg -o $@ --fontfile "./FantasqueSansMono-Regular.otf" -g '205,214,244,255' -b '30,30,46,255'

SRC_FILES := $(wildcard CmdLineMonopoly/*.cpp)
OBJ_FILES := $(patsubst CmdLineMonopoly/%.cpp,bin/%.o,$(SRC_FILES))

bin/CmdLineMonopoly: $(OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $@ $^

bin/%.o: CmdLineMonopoly/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: bin/CmdLineMonopoly
	bin/CmdLineMonopoly

test:
	for i in `find ./CmdLineMonopoly/ -name '*.cpp' -o -name '*.h' | sort`; do echo -e "//`echo "$i" | sed 's,./CmdLineMonopoly/,,'`\n" >> test1.txt && cat "$i" >> test1.txt && echo -e "\n\n" >> test1.txt; done;

screenshots/code-html.html: LICENSE CmdLineMonopoly/CmdLineMonopoly.cpp CmdLineMonopoly/colors.h CmdLineMonopoly/colors.cpp CmdLineMonopoly/utils.h CmdLineMonopoly/utils.cpp CmdLineMonopoly/BoardState.h CmdLineMonopoly/BoardState.cpp CmdLineMonopoly/BoardItems.h CmdLineMonopoly/BoardItems.cpp CmdLineMonopoly/BoardCenter.h CmdLineMonopoly/BoardCenter.cpp CmdLineMonopoly/Player.h CmdLineMonopoly/Player.cpp
	bat $(BAT_PARAMS) $^ --terminal-width 138 | $(AHA) > $@

screenshots/procedure-definition.png: CmdLineMonopoly/BoardState.cpp
	bat $^ -r 170:182 --terminal-width 92 $(BAT_PARAMS) | $(PROJ_REF_FILTER)

screenshots/procedure-usage.png: CmdLineMonopoly/BoardItems.cpp
	bat $^ -r 146:171 -H 157 --terminal-width 115 $(BAT_PARAMS) | $(PROJ_REF_FILTER)

screenshots/list-storage.png: CmdLineMonopoly/BoardState.cpp
	bat $^ -r 129:145 -H 136:138 --terminal-width 102 $(BAT_PARAMS) | $(PROJ_REF_FILTER)

screenshots/list-usage.png: CmdLineMonopoly/BoardState.cpp
	bat $^ -r 147:156 -H 149:150 --terminal-width 54 $(BAT_PARAMS) | $(PROJ_REF_FILTER)

screenshots: screenshots/code-html.html screenshots/procedure-definition.png screenshots/procedure-usage.png screenshots/list-storage.png screenshots/list-usage.png
	static-web-server -p 8080 -d screenshots -z

clean:
	rm -rf bin/*

-include $(OBJ_FILES:.o=.d)