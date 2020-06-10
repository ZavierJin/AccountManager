
FOR /L %%i IN (1,1,7) DO (
  start  RaftTest_7_RandomSleep.exe
  ping -n 2 127.0.0.1>nul
)

