#include "help.h"
#include "options.h"

typedef struct help_t
{
	char txt[4][128];
} help_t;

help_t help[]={
	// 1A - BASIC SHOVING
	{"Here's a simple task - just push the red Barrels",
	 "onto the red squares.  You push them by bumping",
	 "into them.  You can only push, not pull!  If you",
	 "manage to get one stuck, push ESC to retry!",
	},
	{"Barrels come in many different colors.  You can complete the",
	 "level by pushing them all onto the matching colored squares.",
	 "Notice that you can't push a Barrel over a square that is the",
	 "wrong color!  Remember that... it will be important later!",
	},
	{"Notice the raised floor around you?  It's low enough for you",
	 "to walk on, but you can't push barrels onto it at all.  Also",
	 "watch out for the wall blocks - make sure you don't push any",
	 "Barrels into places you can't push them out of!",
	},
	{"Often you will find yourself in tight spaces where every move",
	 "counts.  If you find yourself in a situation you can't fix,",
	 "hit ESC and select Retry!",
	 "",
	},
	{"Each level has different goals to complete.  Complete this",
	 "one by putting six purple Barrels away.  The other Barrels",
	 "are just getting in the way!  Remember how you can't push a",
	 "Barrel over the wrong colored square?  It's important now!",
	},
	// 2A - ROLY POLY
	{"Now you will encounter Balls.  They work just like Barrels,",
	 "except they keep rolling when pushed, until they hit",
	 "something.  This can be tricky!!  If you get one stuck, hit",
	 "ESC and choose Retry.  Put these two Balls away to pass.",
	},
	{"More than with Barrels, you have to plan carefully with",
	 "Balls.  Always watch to be sure that you will have room to",
	 "push the Ball again before you push it into a corner.  It's",
	 "not easy to correct your mistakes like it is with Barrels!",
	},
	{"Remember how you have to plan carefully?  This level will",
	 "require you to do things in a precise order.  Like a",
	 "Barrel, a Ball won't roll over a raised floor or a square",
	 "that is the wrong color.  That is the key to this level!",
	},
	{"This level is extremely easy, but it will show you an",
	 "important concept... a Ball does not have to stop rolling",
	 "to count as a delivery!  But of course, once it rolls off",
	 "of the colored spot, it is no longer delivered. Hmmm...",
	},
	{"You will come across grey objects sometimes also.  A grey",
	 "object pretty much just gets in the way, because there are",
	 "no grey colored squares to deliver it to.  So shove them",
	 "out of your way and solve this big tricky puzzle!",
	},
	// 2B - INVENTORY CLEARANCE
	{"Finally the good stuff!  Pick up Bombs by stepping on them.",
	 "You can place a Bomb in front of yourself by pressing Fire.",
	 "Make sure you are in the right place and facing the right",
	 "way when you do!  Blow up this Barrel to move on.",
	},
	{"You can carry up to 9 Bombs at once.  You use them in the",
	 "order you picked them up.  As you saw, grey Bombs can blow",
	 "up objects of any color.  Colored Bombs, however, only blow",
	 "up objects of the same color.  Don't blow yourself up!",
	},
	{"When an explosion occurs, it will chain to all neighboring",
	 "objects if they are the right color.  Bombs can be blown up",
	 "by an explosion of any color.  This is the key to making",
	 "large chains.  Watch the colors of the explosions.",
	},
	{"This level is extremely easy, but pay attention to what",
	 "the Bombs do.  If you can understand how they work, you will",
	 "get incredible high scores!  Another thing to note: Grey",
	 "objects are totally indestructible.",
	},
	{"Hopefully after this final test, you will have a good basic",
	 "knowledge of how Bombs work.  Can you blow up all of these",
	 "Barrels and Crates with only one Bomb?  Crates work exactly",
	 "like Barrels, only they can't be moved.",
	},
	// 3A - Arrows & Cranks
	{"When you see an Arrow on the floor, that limits the ways you",
	 "can push Barrels.  You can't push a Barrel onto an Arrow Tile",
	 "against the direction it faces, and you can't push one off",
	 "of an Arrow Tile in any direction except the one it faces!",
	},
	{"Like Barrels, Balls have to follow the Arrows.  When you roll",
	 "a Ball, it turns in the direction of any Arrow Tile it rolls",
	 "over.  This level is very easy - just push each Ball to the",
	 "right and get out of their way!",
	},
	{"Turning a Crank will turn all of the Arrow Tiles in the",
	 "level clockwise.  Sometimes you might end up using the same",
	 "Arrows several times facing different directions!  Try to",
	 "deliver these two objects.",
	},
	{"Notice how Bombs can act as obstacles until you pick them",
	 "up. They can also be used as obstacles after you place them,",
	 "but of course, not for long!  A Crank can also serve as an",
	 "obstacle, and notice that the Balls turn it on impact.",
	},
	{"Okay, try to apply all the tricks you've learned to blow up",
	 "all these objects.  Good luck!",
	 "",
	 "",
	},
	// 3B - DOORS & SWITCHES
	{"Those funny pyramid looking things are Doors!  Flip a Switch,",
	 "and all the Doors of the same color will open or close.  This",
	 "level should be a breeze - just blow up all those red objects.",
	 "You'll have to open the Doors to do it.",
	},
	{"Besides Switches, there are other ways to open Doors. In this",
	 "level there are Pressure Pads.  If anything sits on the Pads,",
	 "all the Doors of the same color will be held open or shut.",
	 "Use the Barrel to hold Doors for you and blow up the objects!",
	},
	{"The last way to open Doors is one you probably know well.",
	 "Keys! Pick up Keys by stepping on them, then bump into a Door",
	 "of the same color to open it.  The other trick here is simply",
	 "that you must sometimes hold Pads down with your body weight!",
	},
	{"This level is similar to the last.  But to get past this one,",
	 "you'll need to use expert timing!  Remember sometimes getting",
	 "OFF of a Pad is as important as getting ON one.",
	 "",
	},
	{"This is the final test on Doors.  Your goal here is to blow",
	 "up the blue Barrels.  Be careful when going through the",
	 "Barrel-filled rooms, you can get trapped!  Good luck!",
	 "",
	},
	// 3C - CRITTERS
	{"These Critters are Blobbies. They move at random. Pay close",
	 "attention to what color the Critters are when you meet them.",
	 "Grey ones are just moving obstacles - they are invincible!",
	 "Brown ones are also no threat, but you can blow them up. Do so!",
	},
	{"These Critters are Fluffies. They always turn clockwise when",
	 "they hit an obstacle. Remember, each color of Critter has a",
	 "different behavior - these green ones push objects just like you!",
	 "Watch out for the purple ones! They will eat you!",
	},
	{"These Critters are Inchies. They always turn around when they",
	 "hit an obstacle. Don't forget about the colors! The blue ones",
	 "will turn anything they hit to ice - you can still blow it up,",
	 "but you can't move it. Red ones light any bomb they bump into.",
	},
	{"These Critters are Piggies. They always head straight for you!",
	 "I hope you haven't forgotten what the colors do. Yellow Critters",
	 "destroy objects they bump into.  The aqua colored ones shoot at",
	 "you!",
	},
	{"It would be crazy to combine all the Critter types and colors",
	 "into one level, so here's just a sample.  Dodge past the shots",
	 "of the Aqua Blobbies, escape the Purple Piggies, and grab the",
	 "bombs before the Red Inchy blows them up!",
	},
	// 4A - Gadgets
	{"Besides bombs, there are several other items you can pick up.",
	 "On this level, there are detonators.  They instantly detonate",
	 "all bombs of the same color when you use them.  Remember that",
	 "you use items in the order you pick them up!",
	},
	{"The Shield is an item that is used the instant you pick it up.",
	 "It makes you indestructible for 10 seconds.  Use these to get",
	 "past all those Critters and blow up the green crate!  Life's",
	 "easy when you're indestructible.",
	},
	{"Acid will melt one object of the same color that is directly",
	 "in front of you.  It's not as fun as a bomb, and it can't",
	 "make big chains, but sometimes you need it!  Defeat these",
	 "two Blobbies with the acid.",
	},
	{"Lasers are great! They destroy all objects of the same color",
	 "in a straight line. But even more fun, if they pass over an",
	 "arrow tile, they follow it. This level is easy if you pick up",
	 "the lasers in the right order.",
	},
	{"Okay, now use all the gadgets to make it through this one!",
	 "Should be easy enough, you only have to blow up one measly",
	 "green ball!  There's also one last gadget - the Pencil. It",
	 "doesn't do anything but give you points when you grab it!",
	},
	// 4B - Holes, Pits, & Spikes
	{"This level is a platform suspended over a giant bottomless",
	 "pit.  You can't step into the pit, but any item you push into",
	 "it is gone forever.  You don't get credit for pushing them off",
	 "either! Now destroy the red and blue barrels with one bomb.",
	},
	{"This level is also over a pit.  But it also has Holes.  You",
	 "can't walk over a Hole, but you can push any object into one",
	 "in order to fill it up and make a normal floor.  Critters",
	 "also fill up holes if they fall into them!",
	},
	{"Spikes are deadly to your sensitive feet!  Don't step on them.",
	 "You can still push objects onto them though. Also watch out for",
	 "the one-way doors on this level - the pressure pads make it",
	 "impossible to get back once you've gone through them!",
	},
	{"Of course Spikes are also deadly to Critters.  See if you can",
	 "clean out this room of Critters.  You've gotta move quickly to",
	 "escape the Piggies!  Note also that Balls will roll over",
	 "Spikes, giving you the perfect recipe for Fluffy-Kabobs.",
	},
	{"Here's your final test on this topic.  You must blow up the red",
	 "Fluffy.  Remember: you can push Barrels into the Pit, Cranks",
	 "will turn the Arrows, Blobbies move randomly, and red Critters",
	 "light any bomb they bump into.  Combine your skills!",
	},
	// 4C - Quantum Physics
	{"Here we have teleporters!  They work quite simply.  Step on one",
	 "and you'll immediately appear at another one of the same color.",
	 "Simply use the teleporters to go from room to room and blow up",
	 "all this stuff!",
	},
	{"Critters and objects can teleport just as easily as you can.",
	 "Watch out when you teleport a Barrel though - it will block",
	 "you from using the teleporter again until you move it.",
	 "Also make sure not to teleport into a trap!",
	},
	{"Watch for diamonds on the ground with letters in them.  They",
	 "are Critter Generators!  Every so often a new Critter will",
	 "appear at each Generator.  You can block them by covering",
	 "them up.  Destroy these barrels before the Blobbies get you!",
	},
	{"Critter Generators work at a specific rate. Each level may",
	 "have a different rate. Watch out for these crazy high-speed",
	 "Generators!  Notice that the color and letter of a Generator",
	 "tells you which type of Critter it will create.",
	},
	{"Your final test on Quantum Physics.  A Ball will come out of",
	 "a Teleporter rolling the same way it was when it went in.",
	 "Under that yellow crate is a Teleporter to the lower room.",
	 "Now see if you can blow up all the red objects!",
	},
	// 4D - Scoring
	{"In case you didn't realize, explosions that occur while a",
	 "chain is going on count in that chain. So if you have a",
	 "really long chain going, feel free to blow up some more",
	 "stuff while you wait, it'll boost your score!!",
	},
	{"There are many different goals a level can have.  In this",
	 "one, your goal is to rescue the Critters - get them to go",
	 "into the Exits!  Also, some levels, like this one, have",
	 "time limits.  So get it done fast!!",
	},
	{"Sometimes, levels will have a \"DO NOT\" along with a goal.",
	 "If you do what you're told not to, you will immediately",
	 "lose! In this level, you must defeat the Inchies without",
	 "destroying a single barrel.",
	},
	{"Critters are worth 10 points each, and Bombs are worth only",
	 "1 point each. You don't get points for Bombs that blow up",
	 "normally, though! Only for ones destroyed by a chain explosion.",
	 "If 1 point seems worthless, think about colors and chains!",
	 },
	 {"The total number of points you earn for a chain (5 points",
	 "per Object destroyed, 10 per Critter, and 1 per Bomb) is",
	 "multiplied by the number of different colors that were in the",
	 "chain. Make one chain using ALL of the colors for big points!",
	},
};

void RenderHelpText(int which)
{
	int i;

	for(i=0;i<4;i++)
	{
		CenterPrintOutline(320,250+i*30,help[which].txt[i],0,0);
	}
}
