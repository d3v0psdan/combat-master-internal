
> [!CAUTION]
> <b> Attempting to use anything from this project including methods of anticheat verification, overlay rendering and creating features could result in a PERMANENT SUSPENSION in other game titles or even Combat Master if you use this to intentionally harm the integrity of the game or other players. </b>

![alt text](https://github.com/d3v0psdan/combat-master-internal/blob/main/combat2.png?raw=true)

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
When you've checked the basics and have verified there isn't any sort of commerical anticheat that I've named then your game could have a internal anticheat system. These private internal anticheat systems are usually popular in games that aren't as popular and that don't have as big of a budget as most other games. The easiest way to check the internals for an anticheat is just by attaching a debugger such as OlyDbg, x64/86dbg or CheatEngine. By attaching a debugger it sets certain cpu flags which the internal module would be able to detect, and this is also assuming that the game doesn't have their own in-house kernel anticheat. Though, I have yet to find any small game that would spend the time and effort into creating an in-house kernel anticheat so I believe most would just resort to usermode. 
<br><br>
Once you attach a debugger, if the game does not instantly crash that doesn't mean there isn't an anticheat system in place still. Try setting some breakpoints or using any debugging options you have available to you and see if anything happens if still nothing then it's most likely the game doesn't have an in-house anticheat system. Please keep in mind though, sometimes if an anticheat does detect you, it won't always crash right away and there might be a delay and the game might not crash but rather you might be instantly banned or kicked out of the game, etc. Generally if nothing happens after a few minutes of debugging and messing with the games assembly then you're <b>probably</b> okay.

<br>
<h3><b>3. Conclusion for CombatMaster</b></h3>
Upon starting CombatMaster, there was no evidence of any commerical anticheat. The game is very small (less than 2k players) so I doubt they would want to pay for a commerical anticheat lol. After I started the game, I jumped into a multiplayer match and attached CheatEngine and waited a few minutes to see if anything happened (like if the game crashes or I get kicked), nothing ended up happening so I decided to move onto attaching the CheatEngine debugger and debugging some code and changing values and writing in some assembly and nothing ended up happening. I did also look through the processes modules (using SystemInformer/ProcessHacker) and did not find anything that was blatantly obvious like "anticheat.dll". So for this game, we're all good to go! 


<br>

# 🔓 Overlay Rendering

In order to make our hack a bit more simplier to use, we can make a user GUI menu for all of our features. In order to do this, we need a few things. The first thing we need is a menu framework, so essentially a GUI library to make our menu with and then ofcourse a method to render our overlay. There is multiple different ways games render their graphics, such as OpenGL, Vulkan, and the famous DirectX. In this case for CombatMaster it is being all done with DirectX-11. 
<br><br>
How do I know this? While using SystemInformer, I went through all the modules and found "d3d11.dll" and "GameOverlayRenderer64.dll" which is the DirectX module and steam's 64bit directx hooking library so they can render their ingame overlay and more. 
<br><br>
Now that we know what type of rendering our game is using, we now need to find a way to hook/hijack the rendering to allow us to render our own menu. The most common method in order to hook directx is by simply obtaining the dxgi vtable and swapping out the "Present" function with our own "Present" hook. For reference, the "Present" function is a function run in dxgi.dll every frame to process everything on the screen as far as I'm aware. 
<br><br>
There is a much easier method than just vtable hooking DXGI. We can simply just hook steams rendering overlay since that's what is used in this game.

<br>

# 🔗 Creating features
