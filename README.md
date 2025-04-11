# PACMAN


---

## 🎮 Mô tả dự án

- Xin chào mọi người, mình là Khánh, đây là dự án game đầu tiên mà mình làm. Trong dự án này, mình sẽ làm về game Pacman. Đây là một trò chơi kinh điển được lập trình bằng ngôn ngữ **C++** với thư viện **SDL2**. Dự án mở rộng thêm các tính năng độc đáo như ghost đặc biệt, hệ thống level nâng cao, âm thanh sống động, và nhiều hiệu ứng hình ảnh bắt mắt.

- Hầu hết code trong project mình tham khảo từ ideas của project https://github.com/pvonsite/Pacman
- Link video demo game: https://youtu.be/5RKTMxt5cPY
---

## ⚙️ Cách tải và cài đặt game

1. Đầu tiên, hãy đảm bảo bạn đã cài đặt [Code::Blocks](http://www.codeblocks.org/downloads/26) có hỗ trợ C++.

2. Tiếp theo, tiến hành cài các thư viện cần thiết để game hoạt động:

  - [SDL 2.0](https://github.com/libsdl-org/SDL/releases/latest)
  
  - [SDL_image](https://github.com/libsdl-org/SDL_image/releases/latest)
  
  - [SDL_mixer](https://github.com/libsdl-org/SDL_mixer/releases/latest)

  - [SDL_ttf](https://github.com/libsdl-org/SDL_ttf/releases/latest)

(Bạn có thể tìm hướng dẫn cài đặt SDL cho Code::Blocks trên mạng nếu chưa rõ.)

---

## 🎯 Mục tiêu của trò chơi
    Bạn sẽ vào vai Pacman – một nhân vật màu vàng hình tròn dễ thương. 
    Nhiệm vụ của bạn là ăn hết các dot (coin) trong mê cung để lên level tiếp theo và đạt được số điểm cao nhất có thể. 
    Mỗi người chơi bắt đầu với 3 mạng.

### 🎮 Cách điều khiển
    Dùng phím W A S D hoặc các phím mũi tên để di chuyển Pacman theo 4 hướng.


### 👻 Các đối thủ
    Bạn sẽ bị truy đuổi bởi 4 con ma với hành vi khác nhau:
    Blinky, Pinky, Clyde, Inky – 4 ghost cổ điển từ bản gốc

### ⚡ Power Dot – chấm sức mạnh
    Khi Pacman ăn power dot, bạn sẽ tạm thời khiến các ghost hoảng sợ và có thể ăn ngược lại chúng. 
    Mỗi lần ăn ghost ở trạng thái sợ hãi, bạn sẽ nhận được rất nhiều điểm, và ghost đó sẽ bay về lồng chứa để hồi sinh.

---

## 🧩 Chi tiết về trò chơi
### 🗺️ Bản đồ
    Mê cung trong game có kích thước 28 x 31 ô, tạo thành một không gian di chuyển cố định.
    Mỗi level chứa tổng cộng 244 chấm (dot):
    240 chấm thường (coin nhỏ)
    4 chấm sức mạnh (power dot) đặt ở 4 góc mê cung


### 🔢 Số lượng level
    Trong bản gốc của Pacman, trò chơi có 256 level, nhưng sau level 255 sẽ bị lỗi do giới hạn bộ nhớ 8-bit thời đó.
    Trong phiên bản này, giới hạn level được mở rộng lên tới 2³¹ − 1 (~2.1 tỷ), tương đương với giới hạn của biến int.
    (Dù vậy, chắc cũng không ai đủ kiên nhẫn để chơi đến level 2 tỷ đâu 😄)


### 👻 AI và hành vi của các ghost
-  Các ghost trong game hoạt động dựa trên 3 trạng thái:

        1. Trạng thái đuổi (Chase)
        Mỗi ghost sẽ đuổi theo Pacman theo cách riêng:
        - Blinky: luôn đuổi trực tiếp Pacman
        - Pinky: nhằm tới vị trí trước mặt Pacman 4 ô
        - Inky: tạo điểm đuổi bằng cách đối xứng vị trí Blinky qua Pacman
        - Clyde: nếu cách Pacman hơn 8 ô, sẽ đuổi theo, còn nếu gần thì quay về góc của mình
        
        2. Trạng thái thăm dò (Scatter)
        - Các ghost tạm thời ngừng đuổi Pacman và di chuyển loanh quanh ở một góc nhất định của bản đồ.
        - Đây là khoảng thời gian giúp Pacman dễ thở hơn để gom thêm coin.
        
        3. Trạng thái hoảng sợ (Frightened)
        - Khi Pacman ăn power dot, các ghost sẽ chuyển sang trạng thái sợ hãi (màu xanh).
        - Trong thời gian này, Pacman có thể ăn ghost để nhận thêm điểm.
        - Sau khi bị ăn, ghost sẽ biến thành cặp mắt, quay trở lại lồng để hồi sinh.
        - Hai trạng thái đuổi và thăm dò sẽ luân phiên nhau theo chu kỳ.
        - Trạng thái hoảng sợ chỉ được kích hoạt khi Pacman ăn power dot.


### 📚 Tham khảo thêm về AI ghost tại:
🔗 pacman.fandom.com/wiki/Maze_Ghost_AI_Behaviors


## 🖼️ Nguồn hình ảnh và âm thanh

-  Phần lớn hình ảnh và âm thanh trong project được lấy từ các nguồn tham khảo. Dưới đây là danh sách một số nguồn cụ thể:

    -  [Pixel Art Maker](http://pixelartmaker.com/art/5adcfa718020edf)

    -  [VectorStock – You Lose Stamp](https://www.vectorstock.com/royalty-free-vector/you-lose-rubber-stamp-vector-17695736)
    
    -  [DeviantArt – Dead Heroes Pacman](https://www.deviantart.com/friendbeard/art/Dead-Heroes-Pacman-511878694)
    
    -  [Adobe Stock – Icon phím điều khiển WASD & mũi tên](https://stock.adobe.com/vn/images/keyboard-button-arrow-and-wasd-set-icon-simple-minimal-flat-vector-for-app-and-web/403028677)
    
    -  [Reddit – The heart of the internet](https://www.reddit.com/r/Pacman/comments/10hbu44/comment/j57m04a/?utm_source=share&utm_medium=web2x&context=3)
    
    -  [r/Pacman – I made a full sprite sheet of Sue!](https://www.reddit.com/r/Pacman/comments/xv0pv1/i_made_a_full_sprite_sheet_of_sue/)
    
    -  [Steam Community – Hình screenshot từ hướng dẫn cũ năm 2016](https://steamcommunity.com/sharedfiles/filedetails/?id=2083778829&insideModal=1)
    
    -  [Facebook](https://www.facebook.com/EWScrippsCo/photos/the-society-of-professional-journalists-has-selected-newsy-as-the-recipient-of-a/4781989881816534/)

-  Ngoài ra, một phần nội dung và tài nguyên cũng được kế thừa từ project gốc trên GitHub.

---

## 📁 Cấu trúc thư mục

```bash
Pacman/
├── Assets/              # Hình ảnh, âm thanh game
├── Core/                # File xử lý logic game: game_engine, map, tick_manager...
├── Object/              # Class Pacman, Ghost, Item
├── Menu/                # Các button menu, quản lý trạng thái
├── Manager/             # Quản lý điểm, thời gian, văn bản, âm thanh
├── main.cpp             # File chạy chính
└── README.md            # File mô tả
```

---

## 💡 Ghi chú

- Project được xây dựng phục vụ môn học **Kỹ thuật lập trình Game**
- Mọi tài sản hình ảnh và âm thanh đều là nội bộ hoặc nguồn mở

---

> Mọi đóng góp, nhận xét hoặc bản mở rộng xin hãy để lại Issue hoặc Pull Request trên GitHub!

