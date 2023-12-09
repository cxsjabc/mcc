@echo off

:: ** Please config locally for the file **

:: Pull from gitee or github as your configuration
:: You must config correctly for your local git.

if "%1" == "gitee" (
    git pull origin master
) else if "%1" == "github" (
    git pull github master
) else (
    :: Even use a strange parameter, we'd prefer to believe he wants to push to server, just push!
    git pull origin master
)
