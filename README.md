
# TICTACTOE GAME

### Một phiên bản Caro/Tic-Tac-Toe hiện đại được xây dựng bằng C++ và SFML

<p>
  <img src="https://img.shields.io/badge/C%2B%2B-20-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++20" />
  <img src="https://img.shields.io/badge/SFML-2.x-8CC445?style=for-the-badge&logo=sfml&logoColor=white" alt="SFML" />
  <img src="https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white" alt="Windows" />
  <img src="https://img.shields.io/github/license/simonphucnguyen2304-art/Tictactoe?style=for-the-badge" alt="License" />
</p>

<p>
  <strong>Trải nghiệm những ván Caro chiến thuật với giao diện trực quan, hiệu ứng sinh động, âm thanh tương tác và chế độ chơi với máy.</strong>
</p>

<p>
  <a href="#-giới-thiệu">Giới thiệu</a> •
  <a href="#-tính-năng-nổi-bật">Tính năng</a> •
  <a href="#-cài-đặt-và-chạy-dự-án">Cài đặt</a> •
  <a href="#-điều-khiển">Điều khiển</a> •
  <a href="#-đội-ngũ-phát-triển">Đội ngũ</a>
</p>


---

## Giới thiệu

**CARO GAME** là dự án game bàn cờ viết bằng **C++20**, sử dụng thư viện **SFML** để xây dựng cửa sổ game, đồ họa, âm thanh và tương tác bàn phím.

Dự án hướng đến việc tạo ra một trải nghiệm Caro hoàn chỉnh, không chỉ tập trung vào luật chơi mà còn chú trọng đến giao diện, hiệu ứng chuyển cảnh, âm thanh và khả năng lưu lại tiến trình ván đấu.

> Sẵn sàng thử sức với một đối thủ máy sử dụng thuật toán **Minimax kết hợp Alpha-Beta Pruning**?

## Tính năng nổi bật

- **Chế độ Classic**: Ván Caro truyền thống trên một bàn cờ.
- **Chế độ Extreme**: Biến thể nhiều bàn cờ với bàn cờ lớn tổng hợp.
- **Chơi với máy**: Bot tìm nước đi bằng Minimax và Alpha-Beta Pruning.
- **Chơi 2 người**: Hai người chơi có thể thi đấu trên cùng một máy.
- **Lưu và tải ván đấu**: Quản lý nhiều file save trong thư mục `saves/`.
- **Thống kê người chơi**: Theo dõi điểm số, chuỗi thắng và số lượt đi.
- **Giao diện trực quan**: Menu, nút bấm, hộp thoại và hiệu ứng hover.
- **Hiệu ứng hình ảnh**: Hiệu ứng đặt quân, chiến thắng, hòa và chuyển cảnh.
- **Âm thanh tương tác**: Âm thanh cho thao tác menu, di chuyển, chiến thắng và hòa.
- **Cài đặt trong game**: Có menu tùy chọn, trợ giúp và xác nhận thoát game.

## Công nghệ sử dụng

| Thành phần | Công nghệ |
|---|---|
| Ngôn ngữ | C++20 |
| Đồ họa và cửa sổ | SFML Graphics, SFML Window |
| Âm thanh | SFML Audio |
| Môi trường phát triển | Visual Studio 2022 |
| Build system | Visual Studio `.sln` / `.vcxproj` |
| Nền tảng hiện tại | Windows |

## Cấu trúc dự án

```text
Tictactoe/
├── CAROGAME/
│   ├── Board.cpp / Board.h          # Bàn cờ và luật kiểm tra thắng/thua
│   ├── Bot.cpp / Bot.h              # Đối thủ máy và thuật toán tìm nước đi
│   ├── Game.cpp / Game.h            # Luồng game, chế độ chơi và lưu/tải
│   ├── MainScreen.cpp / .h          # Màn hình chính
│   ├── Menu.cpp / Menu.h            # Menu và các hộp thoại tương tác
│   ├── Setting.cpp / Setting.h      # Cài đặt game
│   ├── Sound.cpp / Sound.h          # Quản lý âm thanh
│   ├── UI.cpp / UI.h                # Thành phần giao diện
│   ├── assets/                      # Texture và tài nguyên hình ảnh
│   ├── audio/                       # Tài nguyên âm thanh
│   ├── font/                        # Font chữ
│   ├── CAROGAME.sln                # Solution Visual Studio
│   └── CAROGAME.vcxproj            # Cấu hình project
├── Report.pdf                       # Báo cáo dự án
├── Sumary.pptx                      # Slide thuyết trình
└── Video.txt                        # Liên kết video demo
```

## Cài đặt và chạy dự án

### Yêu cầu

- Windows 10 hoặc Windows 11.
- Visual Studio 2022 với workload **Desktop development with C++**.
- MSVC v143 hoặc tương thích.
- C++20.
- Thư viện SFML được cấu hình trong thư mục `CAROGAME/SFML`.

### Chạy bằng Visual Studio

1. Clone repository:

   ```bash
   git clone https://github.com/simonphucnguyen2304-art/Tictactoe.git
   cd Tictactoe
   ```

2. Mở file `CAROGAME/CAROGAME.sln` bằng Visual Studio 2022.
3. Chọn cấu hình `Debug` hoặc `Release` và nền tảng `x64`.
4. Đặt project `CAROGAME` làm **Startup Project**.
5. Nhấn `Ctrl + F5` để chạy không debug hoặc `F5` để chạy debug.

> Project hiện được cấu hình cho Windows và sử dụng các thư viện SFML trong thư mục `CAROGAME/SFML`. Nếu môi trường của bạn đặt SFML ở vị trí khác, hãy cập nhật `Additional Include Directories` và `Additional Library Directories` trong cấu hình project.

## Điều khiển

| Phím | Chức năng |
|---|---|
| `W` | Di chuyển lên / chọn mục phía trên |
| `A` | Di chuyển sang trái |
| `S` | Di chuyển xuống / chọn mục phía dưới |
| `D` | Di chuyển sang phải |
| `Enter` | Đặt quân / xác nhận lựa chọn |
| `Esc` | Mở menu tạm dừng trong ván đấu |

## Lưu và tải ván đấu

- Chọn **Save Game** trong menu trong game để lưu ván đấu.
- Các file lưu được tạo trong thư mục `saves/`.
- Từ màn hình quản lý game, bạn có thể tải hoặc xóa ván đấu.
- Nếu chưa nhập tên file, chương trình sẽ tự tạo tên theo thời gian hiện tại.

## Video demo

Xem video giới thiệu và demo dự án tại:

[YouTube Demo](https://youtu.be/GXTvtSnrPPo)

## Tài liệu dự án

- [Báo cáo dự án](./Report.pdf)
- [Slide thuyết trình](./Sumary.pptx)
- [Thông tin video demo](./Video.txt)

## Đội ngũ phát triển

Dự án được thực hiện bởi:

| Thành viên | Vai trò |
|---|---|
| **Nguyễn Trần Hoàng Quân** | Contributor |
| **Nguyễn Kim Phúc** | Contributor |
| **Vũ Đình Anh Kiệt** | Contributor |
| **Huỳnh Đình Khương** | Contributor |
| **Huỳnh Phúc Nguyên** | Contributor |


## License

Dự án hiện chưa khai báo giấy phép mã nguồn mở cụ thể. Vui lòng liên hệ nhóm phát triển trước khi sử dụng hoặc phân phối lại mã nguồn trong sản phẩm khác.
