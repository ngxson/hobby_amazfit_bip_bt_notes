# hobby_amazfit_bip_bt_notes

**ATTENTION: For now, this app support these FW versions: NOT_LATIN 1.1.2.05 and LATIN 1.1.5.36**

This app allows you to save notification as note. Notes can be preserved even after you clear all notifications.  
"BT" means Bluetooth. That means note can be imported through bluetooth instead of typing on your bip (which takes lots of time)

# How to use

**!!!** You should reboot your watch after updated resource file

Watch this video:  
[![demo](https://img.youtube.com/vi/R5pC8uTLgmk/0.jpg)](https://www.youtube.com/watch?v=R5pC8uTLgmk)

**If you want to send directly using @bip prefix**:  
(there is an app to help you, [click here](https://myamazfit.ru/threads/bip-prilozhenija-dlja-bipos-elf.1174/page-111#post-54101))
1. Send a notification from your phone to the watch
2. The title must start with @bip1, @bip2,...
3. Open the app and enjoy

**If you want to save last notification sent to your phone**:
1. Open the app, open a note slot that you want to write into
2. Swipe from right to left
3. Select "Replace with latest notification"
4. Select "OK"

**To delete a note**:
1. Open the app, open a note slot that you want to delete
2. Swipe from right to left
3. Select "DELETE"
4. Select "OK"

# How it works

- The program scan for notifications. The memory address for notification array is found by reversed engineering on the firmware [notification.c](https://github.com/ngxson/hobby_amazfit_bip_bt_notes/blob/main/notification.c)
- It calls `_pvPortMalloc` to create a memory zone for storing notes [utils.c](https://github.com/ngxson/hobby_amazfit_bip_bt_notes/blob/main/utils.c)
- The address to the allocated memory is saved to flash memory [utils.c](https://github.com/ngxson/hobby_amazfit_bip_bt_notes/blob/main/utils.c)  
This memory will not be cleared, unless you reboot the watch.

This is not a good way to store data, but that's the best thing I can do now. Please [see this post](https://myamazfit.ru/threads/bip-application-develop-for-bipos-sdk-en.1171/page-4#post-53688) to know why I can't save notes to flash memory.

# Author
Github: [@ngxson](https://github.com/ngxson)  
Website: [ngxson.com](https://ngxson.com)  

Many thanks to [@MNVolkov](https://github.com/MNVolkov) for his works on BipOS
