
FOR /L %%i IN (1,1,4) DO (
  start  ConsoleApplication19.exe
  ping -n 2 127.0.0.1>nul
)

