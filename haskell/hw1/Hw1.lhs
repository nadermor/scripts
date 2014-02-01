---
title: Homework #1, Due Friday, 1/31/14
---

> module Hw1 where
> import SOE
> import Play
> import XMLTypes

Part 0: All About You
---------------------

Tell us your name, email and student ID, by replacing the respective
strings below

> myName :: [Char]
> myName  = "Qiheng Wang"
> myEmail :: [Char]
> myEmail = "qiw018@cs.ucsd.edu"
> mySID :: [Char]
> mySID   = "A53024431"


Preliminaries
-------------

Before starting this assignment:

* Download and install the [Haskell Platform](http://www.haskell.org/platform/).
* Download the [SOE code bundle](/static/SOE-cse230.tgz).

* Verify that it works by changing into the `SOE/src` directory and
   running `ghci Draw.lhs`, then typing `main0` at the prompt:

~~~
cd SOE/src
ghci Draw.lhs
*Draw> main0
~~~

  You should see a window with some shapes in it.

**NOTE:** If you have trouble installing SOE, [see this page](soe-instructions.html)

5. Download the required files for this assignment: [hw1.tar.gz](/static/hw1.tar.gz).
   Unpack the files and make sure that you can successfully run the main program (in `Main.hs`).
   We've provided a `Makefile`, which you can use if you like. You should see this output:

~~~
Main: Define me!
~~~

Haskell Formalities
-------------------

We declare that this is the Hw1 module and import some libraries:

  module Hw1 where
  import SOE
  import Play
  import XMLTypes

Part 1: Defining and Manipulating Shapes
----------------------------------------

You will write all of your code in the `hw1.lhs` file, in the spaces
indicated. Do not alter the type annotations --- your code must
typecheck with these types to be accepted.

The following are the definitions of shapes:

> data Shape = Rectangle Side Side
>            | Ellipse Radius Radius
>            | RtTriangle Side Side
>            | Polygon [Vertex]
>            deriving Show
>
> type Radius = Float
> type Side   = Float
> type Vertex = (Float, Float)

1. Below, define functions `rectangle` and `rtTriangle` as suggested
   at the end of Section 2.1 (Exercise 2.1). Each should return a Shape
   built with the Polygon constructor.

> rectangle :: Side -> Side -> Shape
> rectangle s1 s2 = Polygon [(x,y), (-x,y), (x,-y), (-x,-y)]
>         where x = s1 / 2
>               y = s2 / 2

> rtTriangle :: Side -> Side -> Shape
> rtTriangle s1 s2 = Polygon [(0,0), (s1,0), (0,s2)]

2. Define a function

> sides :: Shape -> Int
> sides (Rectangle _ _) = 4
> sides (Ellipse _ _) = 42
> sides (RtTriangle _ _) = 3
> sides (Polygon vs) = if length vs > 2
>                      then length vs
>                      else 0

  which returns the number of sides a given shape has.
  For the purposes of this exercise, an ellipse has 42 sides,
  and empty polygons, single points, and lines have zero sides.

3. Define a function

> bigger :: Shape -> Float -> Shape
> bigger s e =
>   case s of Rectangle s1 s2  -> Rectangle (f * s1) (f * s2)
>             Ellipse r1 r2    -> Ellipse (f * r1) (f * r2)
>             RtTriangle s1 s2 -> RtTriangle (f * s1) (f * s2)
>             Polygon vs       -> Polygon (map (\(px,py) -> ((f * px),(f * py))) vs)
>             where f = sqrt(e)

  that takes a shape `s` and expansion factor `e` and returns
  a shape which is the same as (i.e., similar to in the geometric sense)
  `s` but whose area is `e` times the area of `s`.

4. The Towers of Hanoi is a puzzle where you are given three pegs,
   on one of which are stacked $n$ discs in increasing order of size.
   To solve the puzzle, you must move all the discs from the starting peg
   to another by moving only one disc at a time and never stacking
   a larger disc on top of a smaller one.

   To move $n$ discs from peg $a$ to peg $b$ using peg $c$ as temporary storage:

   1. Move $n - 1$ discs from peg $a$ to peg $c$.
   2. Move the remaining disc from peg $a$ to peg $b$.
   3. Move $n - 1$ discs from peg $c$ to peg $b$.

   Write a function

> hanoi :: Int -> String -> String -> String -> IO ()
> hanoi 0 _ _ _  = return ()
> hanoi n a b c = do
>                 hanoi(n-1) a c b
>                 putStrLn( "Move disk from " ++ a ++ " to "++ b)
>                 hanoi(n-1) c b a

  that, given the number of discs $n$ and peg names $a$, $b$, and $c$,
  where a is the starting peg,
  emits the series of moves required to solve the puzzle.
  For example, running `hanoi 2 "a" "b" "c"`

  should emit the text

~~~
move disc from a to c
move disc from a to b
move disc from c to b
~~~

Part 2: Drawing Fractals
------------------------

1. The Sierpinski Carpet is a recursive figure with a structure similar to
   the Sierpinski Triangle discussed in Chapter 3:

![Sierpinski Carpet](/static/scarpet.png)

Write a function `sierpinskiCarpet` that displays this figure on the
screen:

> fillSqu :: Window -> Int -> Int -> Int -> IO ()
> fillSqu w x y size = drawInWindow w (withColor Blue (polygon [(x,y),(x+size,y),(x+size,y+size),(x,y+size),(x,y)]))

> minSize :: Int
> minSize = 3

> sierpinskiSquare :: Window -> Int -> Int -> Int -> IO()
> sierpinskiSquare w x y size = if size <= minSize
>                               then fillSqu w x y size
>                               else let size2 = size `div` 3
>                                    in do sierpinskiSquare w x y size2
>                                          sierpinskiSquare w (x+size2) y size2
>                                          sierpinskiSquare w (x+2*size2) y size2
>                                          sierpinskiSquare w x (y+size2) size2
>                                          sierpinskiSquare w (x+2*size2) (y+size2) size2
>                                          sierpinskiSquare w x (y+2*size2) size2
>                                          sierpinskiSquare w (x+size2) (y+2*size2) size2
>                                          sierpinskiSquare w (x+2*size2) (y+2*size2) size2

> sierpinskiCarpet :: IO ()
> sierpinskiCarpet = runGraphics (
>                      do w <- openWindow "The Sierpinski Carpet" (500, 500)
>                         sierpinskiSquare w 0 0 501
>                         k <- getKey w
>                         closeWindow w )

withColor :: Color -> Graphic -> Graphic
data Color Black | Blue | Green | Cyan | Red | Magenta | Yellow | White

Note that you either need to run your program in `SOE/src` or add this
path to GHC's search path via `-i/path/to/SOE/src/`.
Also, the organization of SOE has changed a bit, so that now you use
`import SOE` instead of `import SOEGraphics`.

2. Write a function `myFractal` which draws a fractal pattern of your
   own design.  Be creative!  The only constraint is that it shows some
   pattern of recursive self-similarity.

> fillCir :: Window -> Int -> Int -> Int -> IO ()
> fillCir w x y size = drawInWindow w (withColor Magenta (ellipse (x,y) (x+size,y+size)))

> sierpinskiCir :: Window -> Int -> Int -> Int -> IO()
> sierpinskiCir w x y size = if size <= 20
>                               then fillCir w x y size
>                               else let size2 = size `div` 3
>                                    in do sierpinskiCir w x y size2
>                                          sierpinskiCir w x (y+size2) size2
>                                          sierpinskiCir w x (y+2*size2) size2
>                                          sierpinskiCir w (x+size2) (y+size2) size2
>                                          sierpinskiCir w (x+2*size2) y size2
>                                          sierpinskiCir w (x+2*size2) (y+size2) size2
>                                          sierpinskiCir w (x+2*size2) (y+2*size2) size2

> myFractal :: IO ()
> myFractal = runGraphics (
>             do w <- openWindow "My Fractal" (500, 500)
>                sierpinskiCir w 0 0 500
>                k <- getKey w
>                closeWindow w )


Part 3: Recursion Etc.
----------------------

First, a warmup. Fill in the implementations for the following functions.

(Your `maxList` and `minList` functions may assume that the lists
they are passed contain at least one element.)

Write a *non-recursive* function to compute the length of a list

> lengthNonRecursive :: [a] -> Int
> lengthNonRecursive xs = foldl (\x _ -> x + 1) 0 xs

`doubleEach [1,20,300,4000]` should return `[2,40,600,8000]`

> doubleEach :: [Int] -> [Int]
> doubleEach [] = []
> doubleEach (x:xs) = (x * 2) : doubleEach xs

Now write a *non-recursive* version of the above.

> doubleEachNonRecursive :: [Int] -> [Int]
> doubleEachNonRecursive xs = map (\x -> x * 2) xs

`pairAndOne [1,20,300]` should return `[(1,2), (20,21), (300,301)]`

> pairAndOne :: [Int] -> [(Int, Int)]
> pairAndOne [] = []
> pairAndOne (x:xs) = (x, x+1) : pairAndOne xs


Now write a *non-recursive* version of the above.

> pairAndOneNonRecursive :: [Int] -> [(Int, Int)]
> pairAndOneNonRecursive xs = zip xs (map (\x -> x + 1) xs)

`addEachPair [(1,2), (20,21), (300,301)]` should return `[3,41,601]`

> addEachPair :: [(Int, Int)] -> [Int]
> addEachPair [] = []
> addEachPair ((a,b):xs) = (a + b) : addEachPair xs

Now write a *non-recursive* version of the above.

> addEachPairNonRecursive :: [(Int, Int)] -> [Int]
> addEachPairNonRecursive xs = map (\(a,b) -> (a + b)) xs

`minList` should return the *smallest* value in the list. You may assume the
input list is *non-empty*.

> minList :: [Int] -> Int
> minList [x] = x
> minList (x:xs) = min x (minList xs)


Now write a *non-recursive* version of the above.

> minListNonRecursive :: [Int] -> Int
> minListNonRecursive (x:xs) = foldr min x xs

`maxList` should return the *largest* value in the list. You may assume the
input list is *non-empty*.

> maxList :: [Int] -> Int
> maxList [x] = x
> maxList (x:xs) = max x (maxList xs)

Now write a *non-recursive* version of the above.

> maxListNonRecursive :: [Int] -> Int
> maxListNonRecursive (x:xs) = foldr max x xs

Now, a few functions for this `Tree` type.

> data Tree a = Leaf a | Branch (Tree a) (Tree a)
>               deriving (Show, Eq)

`fringe t` should return a list of all the values occurring as a `Leaf`.
So: `fringe (Branch (Leaf 1) (Leaf 2))` should return `[1,2]`

> fringe :: Tree a -> [a]
> fringe (Leaf x) = [x]
> fringe (Branch tl tr) = fringe tl ++ fringe tr

`treeSize` should return the number of leaves in the tree.
So: `treeSize (Branch (Leaf 1) (Leaf 2))` should return `2`.

> treeSize :: Tree a -> Int
> treeSize (Leaf _) = 1
> treeSize (Branch tl tr) = treeSize tl + treeSize tr

> -- treeSize t = length (fringe t)

`treeHeight` should return the height of the tree.
So: `height (Branch (Leaf 1) (Leaf 2))` should return `1`.

> treeHeight :: Tree a -> Int
> treeHeight (Leaf _) = 0
> treeHeight (Branch tl tr ) = 1 + max (treeHeight tl) (treeHeight tr)

Now, a tree where the values live at the nodes not the leaf.

> data InternalTree a = ILeaf | IBranch a (InternalTree a) (InternalTree a)
>                       deriving (Show, Eq)

`takeTree n t` should cut off the tree at depth `n`.
So `takeTree 1 (IBranch 1 (IBranch 2 ILeaf ILeaf) (IBranch 3 ILeaf ILeaf)))`
should return `IBranch 1 ILeaf ILeaf`.

> takeTree :: Int -> InternalTree a -> InternalTree a
> takeTree 0 _= ILeaf
> takeTree _ ILeaf= ILeaf
> takeTree n (IBranch a bl br) = IBranch a (takeTree (n-1) bl) (takeTree (n-1) br)

`takeTreeWhile p t` should cut of the tree at the nodes that don't satisfy `p`.
So: `takeTreeWhile (< 3) (IBranch 1 (IBranch 2 ILeaf ILeaf) (IBranch 3 ILeaf ILeaf)))`
should return `(IBranch 1 (IBranch 2 ILeaf ILeaf) ILeaf)`.

> takeTreeWhile :: (a -> Bool) -> InternalTree a -> InternalTree a
> takeTreeWhile _ ILeaf = ILeaf
> takeTreeWhile p (IBranch a bl br) = if (p a)
>                                     then IBranch a (takeTreeWhile p bl) (takeTreeWhile p br)
>                                     else ILeaf

Write the function map in terms of foldr:

> myMap :: (a -> b) -> [a] -> [b]
> myMap f = foldr ((:) . f) []

Part 4: Transforming XML Documents
----------------------------------

The rest of this assignment involves transforming XML documents.
To keep things simple, we will not deal with the full generality of XML,
or with issues of parsing. Instead, we will represent XML documents as
instances of the following simpliﬁed type:

~~~~
data SimpleXML =
   PCDATA String
 | Element ElementName [SimpleXML]
 deriving Show

type ElementName = String
~~~~

That is, a `SimpleXML` value is either a `PCDATA` ("parsed character
data") node containing a string or else an `Element` node containing a
tag and a list of sub-nodes.

The file `Play.hs` contains a sample XML value. To avoid getting into
details of parsing actual XML concrete syntax, we'll work with just
this one value for purposes of this assignment. The XML value in
`Play.hs` has the following structure (in standard XML syntax):

~~~
<PLAY>
  <TITLE>TITLE OF THE PLAY</TITLE>
  <PERSONAE>
    <PERSONA> PERSON1 </PERSONA>
    <PERSONA> PERSON2 </PERSONA>
    ... -- MORE PERSONAE
    </PERSONAE>
  <ACT>
    <TITLE>TITLE OF FIRST ACT</TITLE>
    <SCENE>
      <TITLE>TITLE OF FIRST SCENE</TITLE>
      <SPEECH>
        <SPEAKER> PERSON1 </SPEAKER>
        <LINE>LINE1</LINE>
        <LINE>LINE2</LINE>
        ... -- MORE LINES
      </SPEECH>
      ... -- MORE SPEECHES
    </SCENE>
    ... -- MORE SCENES
  </ACT>
  ... -- MORE ACTS
</PLAY>
~~~

* `sample.html` contains a (very basic) HTML rendition of the same
  information as `Play.hs`. You may want to have a look at it in your
  favorite browser.  The HTML in `sample.html` has the following structure
  (with whitespace added for readability):

~~~
<html>
  <body>
    <h1>TITLE OF THE PLAY</h1>
    <h2>Dramatis Personae</h2>
    PERSON1<br/>
    PERSON2<br/>
    ...
    <h2>TITLE OF THE FIRST ACT</h2>
    <h3>TITLE OF THE FIRST SCENE</h3>
    <b>PERSON1</b><br/>
    LINE1<br/>
    LINE2<br/>
    ...
    <b>PERSON2</b><br/>
    LINE1<br/>
    LINE2<br/>
    ...
    <h3>TITLE OF THE SECOND SCENE</h3>
    <b>PERSON3</b><br/>
    LINE1<br/>
    LINE2<br/>
    ...
  </body>
</html>
~~~

You will write a function `formatPlay` that converts an XML structure
representing a play to another XML structure that, when printed,
yields the HTML speciﬁed above (but with no whitespace except what's
in the textual data in the original XML).

> formatPlay :: SimpleXML -> SimpleXML
> formatPlay xml =
>   head (transform 0 xml)
>     where
>       transform :: Int -> SimpleXML -> [SimpleXML]
>       transform level inputXML =
>         let
>           -- empty tag br
>           br = Element "br" []

>           formatTitle :: Int -> String -> SimpleXML
>           formatTitle level titleName = Element ('h':(show level)) [PCDATA titleName]

>           formList :: Int -> [SimpleXML] -> [SimpleXML]
>           formList _ [] = []
>           formList level (h:t) = (transform level h) ++ (formList level t)
>         in case inputXML of
>           Element "PLAY" body -> [Element "html" [Element "body" (formList (level+1) body)]]
>           Element "TITLE" [PCDATA title] -> [formatTitle level title]
>           Element "PERSONAE" body -> (formatTitle (level+1) "Dramatis Personae"):(formList (level+1) body)
>           Element "PERSONA" personName -> personName ++ [br]
>           Element "LINE" line -> line ++ [br]
>           Element "SPEAKER" personName -> [Element "b" personName , br]
>           Element _ body -> formList (level+1) body
>           _ -> [inputXML]

preorder (Leaf n) = [n]
preorder (Node n a b) = n:(preorder a) ++ (preorder b)

formTitle :: Int -> String -> SimpleXML
formTitle level title = Element ('h':(show level)) [PCDATA title]

The main action that we've provided below will use your function to
generate a file `dream.html` from the sample play. The contents of this
file after your program runs must be character-for-character identical
to `sample.html`.

> mainXML = do writeFile "dream.html" $ xml2string $ formatPlay play
>              testResults "dream.html" "sample.html"
>
> firstDiff :: Eq a => [a] -> [a] -> Maybe ([a],[a])
> firstDiff [] [] = Nothing
> firstDiff (c:cs) (d:ds)
>      | c==d = firstDiff cs ds
>      | otherwise = Just (c:cs, d:ds)
> firstDiff cs ds = Just (cs,ds)
>
> testResults :: String -> String -> IO ()
> testResults file1 file2 = do
>   f1 <- readFile file1
>   f2 <- readFile file2
>   case firstDiff f1 f2 of
>     Nothing -> do
>       putStr "Success!\n"
>     Just (cs,ds) -> do
>       putStr "Results differ: '"
>       putStr (take 20 cs)
>       putStr "' vs '"
>       putStr (take 20 ds)
>       putStr "'\n"

Important: The purpose of this assignment is not just to "get the job
done" --- i.e., to produce the right HTML. A more important goal is to
think about what is a good way to do this job, and jobs like it. To
this end, your solution should be organized into two parts:

1. a collection of generic functions for transforming XML structures
   that have nothing to do with plays, plus

2. a short piece of code (a single deﬁnition or a collection of short
   deﬁnitions) that uses the generic functions to do the particular
   job of transforming a play into HTML.

Obviously, there are many ways to do the ﬁrst part. The main challenge
of the assignment is to ﬁnd a clean design that matches the needs of
the second part.

You will be graded not only on correctness (producing the required
output), but also on the elegance of your solution and the clarity and
readability of your code and documentation.  Style counts.  It is
strongly recommended that you rewrite this part of the assignment a
couple of times: get something working, then step back and see if
there is anything you can abstract out or generalize, rewrite it, then
leave it alone for a few hours or overnight and rewrite it again. Try
to use some of the higher-order programming techniques we've been
discussing in class.

Submission Instructions
-----------------------

* If working with a partner, you should both submit your assignments
  individually.
* Make sure your `hw1.lhs` is accepted by GHC without errors or warnings.
* Attach your `hw1.hs` file in an email to `cse230@goto.ucsd.edu` with the
  subject "HW1" (minus the quotes).
  *This address is unmonitored!*

Credits
-------

This homework is essentially Homeworks 1 & 2 from
<a href="http://www.cis.upenn.edu/~bcpierce/courses/552-2008/index.html">UPenn's CIS 552</a>.
