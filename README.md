### Pineseed platformer game

Pineseed (working codename) is intended to be a 2D platformer adventure game.
Unlike most such games, however, Pineseed is being designed with a specific
focus on de-escalation and on 'conversational combat', where you face down
mighty enemies threatening the local area with naught but your words and your
wits.

Pineseed is currently under development as the authors spend some of their free
time working on creating both the story as well as the engine and editor.

### System requirements

Pineseed is not a particularly resource-intensive game, and as such will
probably run on anything that the game engine,
[Kriti](https://github.com/etherealvisage/kriti), operates on. No support for
mobile platforms is currently planned, though it is an option that the authors
are discussing.

To build the editor, you'll need the Qt 5 development libraries. On Ubuntu,
this is the `qt5-defaults` package.

### Setup and compilation

Edit the file `build.config` to change if you want to build the editor or the
game, or both. Once this has been edited, the command `make redep` will
regenerate the buildsystem; you can also use `make codeblocks` to
create/regenerate a Code::Blocks IDE project file in `build/`.

If you want to build the editor, as noted you'll need Qt5. Beyond that,
compilation should be straightforwards.

To build the game, ensure that the kriti/ git submodule has been initialized,
and then build as normal. Note that you can also replace the submodule with a
symbolic link to another instance of the library somewhere on the filesystem
if you prefer.

So, assuming that you have the prerequisite libraries installed already (see
above), the entire sequence should be something like: (for sh-derived shells,
and modern versions of git)

    $ git clone https://github.com/etherealvisage/pineseed.git
    $ cd pineseed
    Edit build.config file, as appropriate.
    If building game, then run:
    $ git submodule init ; git submodule update
    Either way:
    $ make redep
    $ make -j $NCORES

As noted, you can also use `make codeblocks` to create a Code::Blocks project.

### Contact

* Programming, artwork, and sound: @etherealvisage <ethereal@ethv.net>
* Game design, story: @alastair-JL
