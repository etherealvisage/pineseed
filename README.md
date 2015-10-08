### Pineseed platformer game

Pineseed (working codename) is intended to be a 2D platformer adventure game.
Unlike most such games, however, Pineseed is being designed with a specific
focus on de-escalation and on 'conversational combat', where you face down
mighty enemies threatening the local area with . . . your words and your wits.

Pineseed is currently under development as the authors spend some of their free
time working on creating both the story as well as the engine.

### System requirements

Pineseed is not a particularly resource-intensive game, and as such will
probably run on anything that the game engine,
[Kriti](https://github.com/etherealvisage/kriti), operates on. No support for
mobile platforms is currently planned, though it is an option that the authors
are discussing.

### Setup and compilation

You'll need a compiled instance of kriti, with a version of the kriti source
tree (dist version is fine) available in the `/kriti` project subdirectory.

For example, something like this should work for setting up kriti:

    $ cd ..
    $ git clone https://github.com/etherealvisage/kriti.git
    $ cd kriti
    $ make redep ; make -j [cores]
    $ cd ../pineseed
    $ ln -s ../kriti

Alternatively, you could use an existing precompiled version of kriti. To grab
the precompiled version for x86_64:

    $ wget http://ethv.net/projects/static/kriti-dist.tar.gz
    $ tar xf kriti-dist.tar.gz
    $ ln -s kriti-dist kriti

To compile pineseed:

    $ make redep
    $ make -j [cores]

The command `make redep` will regenerate the buildsystem; you can also use
`make codeblocks` to create/regenerate a Code::Blocks IDE project file in
`build/`.

### Contact

* Programming, artwork, and sound: @etherealvisage <ethereal@ethv.net>
* Game design, story: @alastair-JL
