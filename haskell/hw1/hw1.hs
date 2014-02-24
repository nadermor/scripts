import SOE

data Shape = Rectangle Side Side
           | Ellipse Radius Radius
           | RtTriangle Side Side
           | Polygon [Vertex]
           | Side Side
           | Vertex
             deriving Show

type Radius = Float
type Side = Float
type Vertex = (Float, Float)

sides :: Shape -> Integer
sides (Rectangle s1 s2) = 4
sides (RtTriangle s1 s2) = 3
sides (Ellipse r1 r2) = 42
sides (Side s) = 0
sides (Vertex) = 0
sides (Polygon (v1 : vs)) = 1 + sides(Polygon vs)
sides _ = 0


bigger :: Shape -> Float -> Shape
bigger (Rectangle s1 s2) e =
                     let a = s1 * e
                         b = s2 * e
                             in (Rectangle a b)
bigger (RtTriangle s1 s2) e =
                      let a = s1 * e
                          b = s2 * e
                              in (RtTriangle a b)
bigger (Ellipse s1 s2) e =
                   let a = s1 * e
                       b = s2 * e
                           in (Ellipse a b)
bigger (Side s) e =
            let a = s * e
                    in (Side a)
bigger (Vertex) e = Vertex
bigger (Polygon (v1 : vs)) e = Polygon (calculate (v1 : vs) e)

calculate :: [Vertex] -> Float -> [Vertex]
calculate (v1 : vs) e = let a = multiply v1 e
                                in a : calculate vs e
calculate _ e = []

multiply :: Vertex -> Float -> Vertex
multiply (x1, y1) e =
              let a = x1 * e
                  b = y1 * e
                      in (a, b)



area :: Shape -> Float
area (Rectangle s1 s2) = s1*s2
area (RtTriangle s1 s2) = s1*s2/2
area (Ellipse r1 r2) = pi * r1 * r2
area (Polygon (v1 : vs)) = polyArea vs
                           where polyArea :: [Vertex] -> Float
                                 polyArea (v2 : v3 : vs')  = triArea v1 v2 v3 + polyArea (v3 : vs')
                                 polyArea _ = 0

triArea :: Vertex -> Vertex -> Vertex -> Float
triArea v1 v2 v3 = let a = distBetween v1 v2
                       b = distBetween v2 v3
                       c = distBetween v3 v1
                       s = 0.5 * (a + b + c)
                           in sqrt (s* (s-a) * (s-b) * (s-c))

distBetween :: Vertex -> Vertex -> Float
distBetween (x1, y1) (x2, y2) = sqrt((x1-x2)^2 + (y1-y2)^2)



hanoi :: Int -> String -> String -> String -> IO ()
hanoi n a b c = putStrLn (output (n-1) a b c)

output :: Int -> String -> String -> String -> String
output n a b c = if n==0
                 then "\nmove disk from " ++ a ++ " to " ++ b
                 else output (n-1) a c b ++ "\nmove disk from " ++ a ++ " to " ++ b ++ output (n-1) c b a


