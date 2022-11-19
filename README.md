<div align="center"><img src="https://user-images.githubusercontent.com/87577447/194767433-b6e04d5e-f937-44e9-81a0-c8df37537a1e.jpg" width="200" height="200"/>
<h1>Raiden Shogun Discord Bot</h1>
    <b>
        <p>A new Genshin Impact Discord bot, using Dpp version 10.0.21</p>
    </b>

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/b02c00e09b054b0ca1e3d64227000259)](https://www.codacy.com/gh/Harshfeudal-Projects/Raiden-Shogun/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Harshfeudal-Projects/Raiden-Shogun&amp;utm_campaign=Badge_Grade)
[![GitHub issues](https://img.shields.io/github/issues/Harshfeudal-Projects/Raiden-Shogun)](https://github.com/Harshfeudal-Projects/Raiden-Shogun/issues)
[![GitHub license](https://img.shields.io/github/license/Harshfeudal-Projects/Raiden-Shogun?color=brightgreen)](https://github.com/Harshfeudal-Projects/Raiden-Shogun/blob/main/LICENSE)
[![Discord](https://img.shields.io/discord/900408551573438584?style=flat&logo=discord)](https://discord.gg/6Faaqhaqjs)
<br />
[![Contributor Covenant](https://img.shields.io/badge/Contributor%20Covenant-2.1-4baaaa.svg)](https://github.com/Harshfeudal-Projects/Raiden-Shogun/blob/main/CODE_OF_CONDUCT.md)
[![GitHub stars](https://img.shields.io/github/stars/Harshfeudal-Projects/Raiden-Shogun?color=ff69b4)](https://github.com/Harshfeudal-Projects/Raiden-Shogun/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/Harshfeudal-Projects/Raiden-Shogun?color=ff69b4)](https://github.com/Harshfeudal-Projects/Raiden-Shogun/network)
[![Release](https://img.shields.io/github/v/release/Harshfeudal-Projects/Raiden-Shogun?color=brightgreen)](https://github.com/Harshfeudal-Projects/Raiden-Shogun/releases/latest)
</div>

### :pencil: About this project
- This project is owned by [@harshfeudal](https://github.com/harshfeudal). You can join my Discord server [here](https://discord.gg/BAk2CXpRAT)!
- This repository is under-construction! I'll update it as soon as possible.
- I'm not recommending you to use C++ to code a Discord bot, instead of JavaScript or others library. To know how this project works, you should learn some basics about C++ so that you can know what I am doing.

### 📚 Libraries and version
These are libraries that we're using in this project:
- [Dpp](https://github.com/brainboxdotcc/DPP) - version `10.0.21` (latest)
- [Spdlog](https://github.com/gabime/spdlog) - version `1.11.0` (latest)
- [Base64](https://gist.github.com/tomykaira/f0fd86b6c73063283afe550bc5d77594)
- [SHA256](https://github.com/okdshin/PicoSHA2/blob/master/picosha2.h)

Bot current version: 
- BETA version - `0.1.6.0` (`x64` Release)
- Stable version - `Unknown` (Will be officially released on late December if possible, or at final BETA version - `v0.1.9.0`)

 For downloading **latest version**, please click [here](https://github.com/Harshfeudal-Projects/Raiden-Shogun/releases).

### 🚨 Generate and use
 This project was made using Visual Studio, so basically you cannot see my `.sln` files because I have ignored it. To use this project, you should generate it by using [Premake](https://premake.github.io/).

 So basically, I have written `premake5.lua` for generating it. Here are these steps:
 1. Download `premake5.exe` from [their website](https://premake.github.io/)
 2. Open the folder where you downloaded this project onto, put the `premake5.exe` file into this project (Insert the file to the directory where `Raiden Shogun.rc` is located)
 3. On the navigation bar, for example `D:/Harshfeudal/Raiden Shogun/`, clear all and type `cmd` on that, then enter and the terminal will release
 4. On that terminal, type:
 ```
 $ premake5 vs[version]
 ```
 Example: If you're using Visual Studio 2022 like me, just do it like this:
 ```
 $ premake5 vs2022
 ```
 5. Rename the `.example.config.json` file to `config.json`. Add your bot token and client secret in that file:
 ```json
 {
    "token": "[add your bot token in here]",
    "client_secret": "[add your bot secret in here]"
 }
 ```
 6. Build, run, and enjoy!

 **New version note:** Please add your token after you encrypt it into `Base64`. For encryption, you can use online Base64 converter or use my [library](https://github.com/Harshfeudal-Projects/Raiden-Shogun/blob/main/include/harshfeudal/base64.h)!

 ### 📜 Some notes
 This project only supports:
 - Microsoft Windows 10 `x64`
 - Visual Studio platform `x64`
 - C++ 17 `ISO/IEC 14882`
 - Please install `.dll` files on [dpp page](https://dpp.dev/) and put it near your `.exe` executable file so that it will run well

 In other cases it crashes or doesn't compile.

### 👷 Code platform
- Visual Studio Enterprise 2022
- Visual Studio Code

### 💎 Future plan
- [ ] CMake support soon!
- [x] Add `premake5.lua`
- [x] Code works well (currently)

 ### 🤝 Support me
 - Please leave me a star if you like it. Thank you very much!
 - You can probably help me develop this project too by DMing me on my [Discord profile (Harshfeudal#0001)](https://discord.com/users/622450109317251088).

 ### :computer: Contribution and Pull Request
  I'd probably glad to see if you have anything new to help and support me. To do that, please make one and before you do that, just make sure you've tested the code by using your bot. I won't bite you if you do it wrong, but just make sure that you have to test it clearly before I merge it.

 ### :sparkling_heart: Special thanks
  - My bot avatar artist (Please contact me if you're the owner of that avatar so that I can link your resource).
  - Genshin Impact game for character name (Raiden Shogun)
  - My companions on D++ Discord server!
  - My friends
  - And you too, for staring this project and encourage me to do it!
  - [DarkCheese_](https://github.com/VeryHolyCheeeese) for fixing the grammar
