module Shape where
import SOE
import Play
import XMLTypes


data Shape = Rectangle Side Side
           | Ellipse Radius Radius
           | RtTriangle Side Side
           | Polygon [Vertex]
deriving Show

type Radius = Float
type Side   = Float
type Vertex = (Float, Float)

rectangle :: Side -> Side -> Shape
rectangle s1 s2 = Polygon [(x,y), (-x,y), (x,-y), (-x,-y)]
        where x = s1 / 2
              y = s2 / 2

rtTriangle :: Side -> Side -> Shape
rtTriangle s1 s2 = Polygon [(0,0), (s1,0), (0,s2)]


sides :: Shape -> Int
sides (Rectangle _ _) = 4
sides (Ellipse _ _) = 42
sides (RtTriangle _ _) = 3
sides (Polygon ss) = if length ss > 2
                     then length ss
                     else 0

bigger :: Shape -> Float -> Shape
bigger s e = error "Define me!"

