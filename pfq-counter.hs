module Main where

import Network.PFq as Q

import Foreign
import Data.Maybe
import System.Environment
import System.Time

import Control.Monad
import Control.Concurrent

recvLoop :: (Num a) => Ptr PFqTag -> MVar a -> IO Int
recvLoop q counter = do 
    netQueue <- Q.read q 10000
    case (Q.queueLen netQueue) of 
        0 ->  recvLoop q counter 
        _ ->  do
              modifyMVar_ counter $ \c -> return (c + fromIntegral (Q.queueLen netQueue))
              recvLoop q counter


launcher :: (Num a) => String -> Int -> Maybe String -> IO [MVar a]
launcher _ 0 _ = return []
launcher dev n steer = do 
         c <- newMVar 0
         _ <- forkOn (n-1) (
                                do
                                fp <- Q.openNoGroup 46 14 131000
                                withForeignPtr fp  $ \q -> do
                                    Q.joinGroup q 42 [Q.class_default] Q.policy_shared
                                    Q.bindGroup q 42 dev (-1)
                                    Q.enable q 
                                    when (isJust steer) (Q.steeringFunction q 42 (fromJust steer))
                                    recvLoop q c >> return ()  
                           )
         putStrLn $ "#" ++ show (n-1) ++ " started!"
         liftM2 (:) (return c) (launcher dev (n-1) steer)
                         

dumper :: String -> Int -> Maybe String -> IO ()
dumper dev n steer = do
        cs <- launcher dev n steer 
        t  <- getClockTime
        dumpStat cs t


diffUSec :: ClockTime -> ClockTime -> Int
diffUSec t1 t0 = (tdSec delta * 1000000) + truncate ((fromIntegral(tdPicosec delta) / 1000000) :: Double)
                    where delta = diffClockTimes t1 t0


dumpStat :: (RealFrac a) => [MVar a] -> ClockTime -> IO ()
dumpStat cs t0 = do
             threadDelay 1000000
             t <- getClockTime
             cs' <- mapM (\v -> swapMVar v 0) cs
             let delta = diffUSec t t0
             let rate = (sum cs' * 1000000) / fromIntegral delta  
             putStrLn $ "total rate pkt/sec: " ++ show ((truncate rate) :: Integer)
             dumpStat cs t

main :: IO ()
main = do
    args <- getArgs
    case (length args) of
        0   -> error "usage: pfq-counter dev #thread <steer-function>"
        1   -> error "usage: pfq-counter dev #thread <steer-function>"
        2   -> dumper (args !! 0) (Prelude.read(args !! 1) :: Int) Nothing
        _   -> dumper (args !! 0) (Prelude.read(args !! 1) :: Int) (Just (args !! 2))



