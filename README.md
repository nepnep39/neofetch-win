# neofetch-win #

Simple, ultra-lightweight neofetch clone for Windows 10+ written in C++ and installable through winget. The executable is just under 75KB and requires no dependencies other than the MS Visual C++ redists (which should be installed already, and if they're not my installer will take care of that for you :)

2024-03-27 - Thanks for 50 000 downloads!! 🥳🎉

## Install ##

`winget install neofetch`

`neofetch`

That's it

## Build ##

Used to only compile in C++20, now should build with older standards (tested and working for C++11 and 14)

Run from project root directory
`msbuild.exe /p:IncludePath=. /clp:ErrorsOnly /p:Configuration=Release`

## Pictures ##

![neofetch121](https://github.com/nepnep39/neofetch-win/assets/119973523/bf25c8af-4dba-445f-b515-2629b559facb)

![win10](https://user-images.githubusercontent.com/119973523/222497683-b473a644-3bb7-43fb-8bbc-ff5bf3a87481.png)

![win11](https://user-images.githubusercontent.com/119973523/224430965-30442685-638f-4096-8579-b97700b419e6.png)

Evil Jay — Today at 7:05 PM
Well you're gonna have a hard time selling your bootleg neofetch :stoichSlow:

made with love by nepnep

