factorial :: Integer -> Integer
factorial n = product [1..n]

factorialInt :: Int -> Int
factorialInt n = product [1..n]
-- Int is smaller than Integer, might overflow

pos :: Integer -> Bool
pos x = (x > 0)

-- pat :: Int -> Int -> Int -> Bool
