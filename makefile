# This is the main script that compiles everything for your game. 
# There are some settings up at the top you can change, but hopefully you will not have to touch this 
# otherwise.
#

# ===== USER SETTINGS START HERE =====

# The name of the output rom, without the trailing .nes.
ROM_NAME=puzzle

# ===== USER SETTINGS END HERE =====


# ===== ENGINE FUNCTIONS START HERE =====
# Hat-tip: https://stackoverflow.com/questions/2483182/recursive-wildcards-in-gnu-make/18258352#18258352
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
# ===== ENGINE FUNCTIONS END HERE =====

# ===== ENGINE SETTINGS START HERE =====
# Note: You can edit these if you know what you're doing, but hopefully you don't have to. 
MAIN_COMPILER=./tools/cc65/bin/cc65
MAIN_ASM_COMPILER=./tools/cc65/bin/ca65
MAIN_LINKER=./tools/cc65/bin/ld65
SPACE_CHECKER=tools/nessc/nessc
SFX_CONVERTER=tools/neslib_famitracker/tools/nsf2data
AFTER_SFX_CONVERTER=mv sound/sfx/sfx.s sound/sfx/generated/sfx.s

# Built-in tools: 
CHR2IMG=tools/chr2img/chr2img
SPRITE_DEF2IMG=tools/sprite_def2img/sprite_def2img

# Javascript versions of built-in tools: (Uncomment these if you're working on the tools)
# CHR2IMG=node tools/chr2img/src/index.js
# SPRITE_DEF2IMG=node tools/sprite_def2img/src/index.js

SOUND_BANK=0

SOURCE_C=$(strip $(call rwildcard, source/, *.c))
SOURCE_S=$(patsubst source/, temp/, $(patsubst %.c, %.s, $(SOURCE_C)))
SOURCE_O=$(addprefix temp/, $(notdir $(patsubst %.s, %.o, $(SOURCE_S))))
SOURCE_DIRS=$(sort $(dir $(call rwildcard, source, %))) temp
SOURCE_CRT0_ASM=$(strip $(call rwildcard, source/, *.asm))
SOURCE_CRT0_GRAPHICS=$(strip $(call rwildcard, graphics/, *.pal)) $(strip $(call rwildcard, graphics/, *.chr))
SOURCE_HEADERS=$(strip $(call rwildcard, source/, *.h))

VPATH=$(SOURCE_DIRS)
# Uses the windows command line to open your rom, 
# which effectively does the same thing as double-clicking the rom in explorer.
MAIN_EMULATOR=cmd /c start

# Which configuration file to use. Lets you add more PRG banks and/or SRAM.
CONFIG_FILE_PATH=tools/cc65_config/game

# These are the configuration files loaded by the line above. You probably don't have to change these.
CONFIG_FILE=$(CONFIG_FILE_PATH).cfg

# Path to 7-Zip - only used for generating tools zip. There's a 99.9% chance you don't care about this.
7ZIP="/cygdrive/c/Program Files/7-Zip/7z"
# ===== ENGINE SETTINGS END HERE =====

# ===== Actual makefile logic starts here =====
# You really shouldn't need to edit anything below this line if you're not doing advanced stuff.

# Tweak a bunch of stuff to use circleci settings
# These might get you running in a limited fashion on non-windows systems too...
ifdef CIRCLECI
	# SFX and music are committed to git, so we don't use these tools. They're sadly windows-only anyway...
	SFX_CONVERTER=echo -q
	AFTER_SFX_CONVERTER=echo Skipping SFX Generation...
	# We don't have our nice toolkit here, so we have to use node directly
	CHR2IMG=node tools/chr2img/src/index.js
	SPRITE_DEF2IMG=node tools/sprite_def2img/src/index.js

endif

# Cancelling a couple implicit rules that confuse us greatly
%.o : %.s
%.o : %.c
%.s : %.c

# Keeping all intermediate files, so debugging and learning is easier. :)
# There are probably better ways to do this, and I know things are still deleted. Feel free to PR a better way in.
.PRECIOUS: $(SOURCE_C) $(SOURCE_S) $(SOURCE_O) temp/%.s

build: rom/$(ROM_NAME).nes 

temp/base.asm: source/neslib_asm/crt0.asm
	echo ".include \"source/neslib_asm/crt0.asm\"" >> temp/base.asm

temp/crt0.o: source/neslib_asm/crt0.asm source/neslib_asm/neslib.asm temp/base.asm $(SOURCE_CRT0_GRAPHICS) sound/music/music.bin sound/music/samples.bin sound/sfx/generated/sfx.s source/library/patchable_data.asm
	$(MAIN_ASM_COMPILER) -I . temp/base.asm -o temp/crt0.o -D SOUND_BANK=$(SOUND_BANK) --debug-info

# This bit is a little cheap... any time a header file changes, just recompile all C files. There might
# be some trickery we could do to find all C files that actually care, but this compiles fast enough that 
# it shouldn't be a huge deal.
temp/%.s: %.c $(SOURCE_HEADERS)
	$(MAIN_COMPILER) -I . -Oi $< --add-source  --include-dir ./tools/cc65/include -o $(patsubst %.o, %.s, $@) --debug-info

temp/%.o: temp/%.s
	$(MAIN_ASM_COMPILER) -I . $< --debug-info

temp/%.s: temp/%.c
	$(MAIN_COMPILER) -Oi $< --add-source --include-dir ./tools/cc65/include -o $(patsubst %.o, %.s, $@) --debug-info

sound/sfx/generated/sfx.s: sound/sfx/sfx.nsf
	$(SFX_CONVERTER) sound/sfx/sfx.nsf -ca65 -ntsc && sleep 1 && $(AFTER_SFX_CONVERTER)

rom/$(ROM_NAME).nes: temp/crt0.o $(SOURCE_O)
	$(MAIN_LINKER) -C $(CONFIG_FILE) -o rom/$(ROM_NAME).nes temp/*.o tools/cc65/lib/nes.lib  --dbgfile rom/$(ROM_NAME).dbg

s3_upload:
	mc cp ./rom/puzzle.nes  s3/cpprograms-nes-games-https/retro-puzzle-maker-v5-1.nes && mc policy public s3/cpprograms-nes-games-https/retro-puzzle-maker-v5-1.nes

clean:
	-rm -f rom/*.nes
	-rm -f temp/*
	-rm -f sounds/sfx/generated/*.s
	-rm -f graphics/generated/*.png
	touch temp/empty

run:
	$(MAIN_EMULATOR) rom/$(ROM_NAME).nes

space_check:
	$(SPACE_CHECKER) rom/$(ROM_NAME).nes
