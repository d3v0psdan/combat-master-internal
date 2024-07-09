![alt text](https://github.com/d3v0psdan/combat-master-internal/blob/main/images/combat2.png?raw=true)

# 🔎 Overview
Combat Master is a tactical FPS free-to-play multiplayer game on steam. Using my current knowledge of reverse engineering and C++ this project is an aim to develop a educational simple internal hack menu for Combat Master.
<b>This project is strictly for educational and learning purposes and is not intended to harm the integrity of the game or anyone playing it.</b>

<br>

# 🔗 Structure (DLL)
* <h3><b>main.cpp: Everything directx & hooking related happens.</b></h3>
```cpp
// Handle menu & game keyboard inputs.
LRESULT inputHandler(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Initialize directx variables & handle present hook.
HRESULT presentHooked(IDXGISwapChain* swapChain, UINT sync, UINT flag);

// Initialize variables & hook directx.
DWORD WINAPI MainThread();
```
* <h3><b>menu.cpp: Where everything hacking related is executed.</b></h3>
```cpp
// Initialize & handle GUI/Menu colors/style/theme.
void uiMenu::Styles();

// Handle GUI/Menu rendering
void uiMenu::Render();

// Handles all hacking events even while menu is not open, runs in a constant loop.
void uiMenu::Background();
```

* <h3><b>game.cpp: Where the features/hack variables/functions are defined.</b></h3>
```cpp
// Gets the DLL base address of the module needed to make our features.
uint64_t getDllBase();

// Simple verification to ensure player is in game before allowing features to execute.
void isInGame(bool& value);
void toggleInfiniteAmmo(bool value); // Infinite ammo hack
void toggleInfiniteMag(bool value); // Infinite mag hack
void toggleInfiniteGrenades(bool value); // Infinite grenades hack
void setWorldFov(float value); // Set custom world field of view
void setHandsFov(float value); // Set custom hands field of view
```

# Reverse Engineering

<h3>This project would not have been possible without reverse engineering, so in this section I will dive into how I managed to create different aspects of this project through reverse engineering.</h3>

* <h2><b>Creating features (infinite grenades)</b></h2>
<h4>Creating the actual hacks for this game wasn't too hard but was just time consuming because it's quite repetitive. CombatMaster was made with UnityEngine with the IL2CPP option and it's quite easy to hack UnityEngine games that use IL2CPP but the only issue is with this game the "global-metadata.dat" file is encrypted and loaded into the games memory I believe. At this point in time, I haven't figured out a solution to decrypt the global-metadata.dat. So the solution for now is just to use CheatEngine and pointer scan for the things we want to mess around with. In this example I'll show my method I used to achieve getting infinite grenades. This method is the same thing I applied to the rest of the features.</h4>
1. Attach CheatEngine to the process and join a private lobby with BOTs only.
2. Search for the current value of our grenades (which is 2) then keep throwing grenades until the value goes all the way down to 0.

![alt text](https://github.com/d3v0psdan/combat-master-internal/blob/main/images/RE_10.png?raw=true)
![alt text](https://github.com/d3v0psdan/combat-master-internal/blob/main/images/RE_11.png?raw=true)
3. Now since we have 0 grenades, most of the values are still continuing to change in our address list. So what we are going to do is continue spamming the "Next Scan" button until we narrow down our results to as low as possible.

![alt text](https://github.com/d3v0psdan/combat-master-internal/blob/main/images/RE_12.png?raw=true)
4. I was able to narrow down the results to 11. Moving around and doing other things in the game will help lower this value even further. So now what we want to do is just change each value back to 2 instead of something crazy to reduce the risk of the game crashing. Starting from the bottom of the address list, change each value back to 2 and watch your grenades count. The 3rd last address in my case was the grenades!

![alt text](https://github.com/d3v0psdan/combat-master-internal/blob/main/images/RE_13.png?raw=true)
5. Now that we have our grenades address, let's create a pointer map to it. Essentially, just a chain of pointers leading to that memory so we can have a static address pointing to it so no matter if we restart the game we will have a consistent address pointing to the grenades amount.

* <h2><b>Overlay/Directx Hooking:</b></h2>
<h4>One of the most common methods of hooking directx is through obtaining the IDXGISwapChain vtable in dxgi.dll. I personally don't like to use this method because it just takes more effort and personally and there is a much easier way to do it. Whenever you startup a game, especially if through steam. Steam automatically loads their in-game overlay to allow you to manage things through steam easier without tabbing out. We can easily take advantage of this through hooking their overlay that hooks directx. Below are the steps I took to hook it and render my own menu. I would like to say that I am not the first person who has ever done this, people have thought of this much before I have so I just took it upon myself to figure it out without any help since that's what makes it fun.</h4>

1. Open up "GameOverlayRenderer64.dll" in a reverse engineering software of your choice and load up the strings and search for the string `"vtable"`.
![alt text](https://github.com/d3v0psdan/combat-master-internal/blob/main/images/RE_1.png?raw=true)
![alt text](https://github.com/d3v0psdan/combat-master-internal/blob/main/images/RE_2.png?raw=true)
2. Once you found the string, I clicked on the first one then cross-referenced it to a function when then I went into.
![alt text](https://github.com/d3v0psdan/combat-master-internal/blob/main/images/RE_3.png?raw=true)
3. Now that we're inside the function, the reverse engineering software has labeled some functions for us because of string references. It would be safe to assume that one of these functions that are being hooked is the present function, since that is a function that needs to be hooked in order to render objects on the screen. So how do we figure out which function(sub_xxxxx/qword_xxxxx) is it?
4. To identifiy which function it is, present has 2 arguments [IDXGISwapChain::Present method (dxgi.h)](https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgiswapchain-present). But in reality it actually has 3 arguments when disassembling the code because the actual first argument is the reference/pointer to the IDXGISwapChain class instance. So now that we know we're searching for 3 arguments with a return value of a `HRESULT` lets start looking at each of those `sub_xxxxx()` hook detours. 
   
![alt text](https://github.com/d3v0psdan/combat-master-internal/blob/main/images/RE_4.png?raw=true)
![alt text](https://github.com/d3v0psdan/combat-master-internal/blob/main/images/RE_5.png?raw=true)
![alt text](https://github.com/d3v0psdan/combat-master-internal/blob/main/images/RE_6.png?raw=true)
![alt text](https://github.com/d3v0psdan/combat-master-internal/blob/main/images/RE_7.png?raw=true)

5. We can see that there is 4 functions total, but we can actually narrow it down to just 2 functions. Here's why: the first function only has one argument so we can skip that, but the rest of the functions each have 3 arguments. So what is the solution to know which one it is? We just have to manually test! So what we will need to do to make this easy, is when you go into the function. The return value is always returned by calling the `qword_xxxxx` variable (the original function), so why don't we just exchange the original function to point to our own function? (This is also called a .data pointer swap/hook). We will test each individual address until one of them works.
![alt text](https://github.com/d3v0psdan/combat-master-internal/blob/main/images/RE_8.png?raw=true)   

6. Let's test the first one and see if it works. Would you look at that! it worked!
![alt text](https://github.com/d3v0psdan/combat-master-internal/blob/main/images/RE_9.png?raw=true)

8. So thats it for the steam overlay hook. We got lucky and the first function worked, but if it didn't we would just test the rest of the addresses.

# 🔗 Structure (Injector)
* <h3><b>main.cpp: Simple LoadLibraryA injection.</b></h3>
```cpp
// Error handling function.
void fatalError(std::string message);

// Function to obtain process ID of CombatMaster.
DWORD getPid(const char* processName);

// Function that injects DLL into CombatMaster through obtaining
// processID and DLL path and writing all the data into the process and then
// remotely calling "LoadLibraryA" inside the process. 
int main();
```
* <h3><b>How does this injection method work?:</b></h2>
<h4>Whenever you start up a game, it automatically loads DLL's the game requires. Our DLL/Hack is not natively apart of the game, so we need to forcefully load ours in because the game does not just load ours in for us.</h4>

<h4>How does the game automatically load in it's DLL's? Typically most games will use the function from <b>kernel32.dll</b> called <b>LoadLibraryA</b>(https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibrarya).
So all we need to do is just call that function in order to load in our module! So let's get right into that.
</h4>

<h4>Because we do not have direct access to the games memory, we will need to create our own process which attaches to the game and injects code to remotely call the LoadLibraryA function. So for example, if we had direct access to the games memory (like we were a DLL inside the game) we could simply execute code like this:</h4>

```cpp
#include <Windows.h>

void loadCustomDLL()
{
  auto loadedDLL = LoadLibraryA("C:\\Users\\example\\my_hack.dll");
  if (loadedDLL)
      printf("Loaded DLL! %p\n", loadedDLL);
}

loadCustomDLL();
```

<h4>But since we don't have direct access to the games memory, we need to call this function remotely through injecting memory (known as shellcode) and executing it. Here is how we can achieve this result:</h4>

1. Obtain access to process through opening a handle with all access for reading/writing/executing memory. Using `OpenProcess()`
2. Allocate memory to store our DLL's path, since that is a parameter of LoadLibraryA. Using `VirtualAllocEx()`
3. Write our DLL's path to our allocated memory. Using `WriteProcessMemory()`
4. Execute the `CreateRemoteThread()` function to call LoadLibraryA inside of the process and pass our allocated memory as the starting parameter, so when we call LoadLibraryA we pass the address of our string which is loaded into the games memory.
5. Wait for our DLL to execute it's `DllMain()` using `WaitForSingleObject()`.
6. Close all handles and cleanup memory, using `CloseHandle()` and `VirtualFreeEx()`.
