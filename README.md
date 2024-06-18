> [!CAUTION]
> <b> Attempting to use anything from this project including methods of anticheat verification, overlay rendering and creating features could result in a PERMANENT SUSPENSION in other game titles or even Combat Master if you use this to intentionally harm the integrity of the game or other players. </b>

# 🔎 Overview
Combat Master is a tactical FPS free-to-play multiplayer game on steam. Using my current knowledge of reverse engineering and C++ this project is an aim to develop a educational simple internal hack menu for Combat Master.
<b>This project is strictly for educational and learning purposes and is not intended to harm the integrity of the game or anyone playing it.</b>

<h3>⬇️ Defining our objectives:</h3> 
<b>1. Check and verify that the game does not have any anti-cheat systems.</b><br>
<b>2. Hook directx to render our overlay.</b><br>
<b>3. Create multiple different features including infinite ammo, grenades and more.</b>

<h4>Below is each objective and how it's completed.</h4>
<br>

# 🔐 Anticheat Verification
Let's dive right in! So the first step in order to detect any anticheat systems is by doing the basics first. 
<br>
<h3><b>1. Checking the basics</b></h3>
The most common commerical anticheats today are <b>EasyAntiCheat, BattlEye, Valve-AntiCheat (VAC)</b>. I'm not going to name every single anticheat on the mark but some others are <b>RIOT Vanguard, Ricochet, EQU8</b>. These anticheats though are only really game specific, RIOT with Valorant and Ricochet for Call Of Duty and EQU8 is a broad range of games but I've only seen it one a unity game on steam and Splitgate.
<br><br>
For this project, we're only covering the most common anticheats. The first step to check them is by using Task Manager or what I prefer, SystemInformer (formally known as ProcessHacker).
<br>
So in order to check for the most common anticheats, it's incredibily easy. We're just going to look for the first two I've listed, EasyAntiCheat and BattlEye. VAC is typically just used only in Valve developed games and it's DLL that's remotely streamed. You can read more about VAC here (https://github.com/danielkrupinski/VAC). 
<br><br>
<b>When starting up your game of choice, typically you see either the EasyAntiCheat or BattlEye splashscreen/launcher appear. If you don't see anything like this popping up upon starting your game then your game most likely doesn't have either of these anticheats. In order to verify if your game is running any of these anticheats, simply goto Task Manager and look for any processes containing the name "EasyAntiCheat" or "BEService". You can also goto the services section in Task Manager to check if these drivers are running and if they are, your game is running either of these anticheats.</b>

<h3><b>2. Checking the internals </b></h3>
When you've checked the basics and have verified there isn't any sort of commerical anticheat that I've named then your game could have a internal anticheat system. These private internal anticheat systems are usually popular in games that aren't as popular and that don't have as big of a budget as most other games.

<br>

# 🔓 Overlay Rendering

<br>

# 🔗 Creating features
