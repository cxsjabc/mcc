@echo off

:: ** Please config locally for the file **

:: Push to gitee or github as your configuration
:: You must config correctly for your local git.

if "%1" == "gitee" (
    git push origin master
) else if "%1" == "github" (
    git push github master
) else (
    :: Even use a strange parameter, we'd prefer to believe he wants to push to server, just push!
    git push origin master
)
