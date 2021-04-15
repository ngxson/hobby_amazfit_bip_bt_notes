# hobby_amazfit_bip_bt_notes

**ATTENTION: For now, this app only support FW version NOT_LATIN 1.1.2.05**

This app allows you to save notification as note. Notes can be preserved even after you clear all notifications.  
"BT" means Bluetooth. That means note can be imported through bluetooth instead of typing on your bip (which takes lots of time)

# How to use

**!!!** You should reboot your watch after updated resource file

Watch this video:  
[![demo](https://img.youtube.com/vi/2Wkw4q010VU/0.jpg)](https://www.youtube.com/watch?v=2Wkw4q010VU)

1. Send a notification from your phone to the watch
2. The title must start with @bip1, @bip2,...
3. Open the app and enjoy

# How it works

- The program scan for notifications. The memory address for notification array is founded by reversed engineering the firmware [notification.c#L11](https://github.com/ngxson/hobby_amazfit_bip_bt_notes/blob/main/notification.c#L11)
- It calls `_pvPortMalloc` to create a memory zone for storing texts [utils.c#L29](https://github.com/ngxson/hobby_amazfit_bip_bt_notes/blob/main/utils.c#L29)
- The address to the allocated memory is saved to flash memory [utils.c#L31](https://github.com/ngxson/hobby_amazfit_bip_bt_notes/blob/main/utils.c#L31)  
This memory will not be cleared, unless you reboot the watch.

This is not a good way to store data, but that's the best thing I can do now. Please [see this post](https://myamazfit.ru/threads/bip-application-develop-for-bipos-sdk-en.1171/page-4#post-53688) to know why I can't save notes to flash memory.

# Author
Github: [@ngxson](https://github.com/ngxson)  
Website: [ngxson.com](https://ngxson.com)  

Many thanks to [@MNVolkov](https://github.com/MNVolkov) for his works on BipOS
