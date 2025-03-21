# 🎮 숫자 야구 게임 - Unreal Engine & C++
숫자 야구 게임은 서버가 랜덤으로 생성한 3자리 숫자를 각 플레이어들이 맞추는 게임입니다. <br>

<br>

### 📌 01. 게임 규칙
- 서버는 **서로 다른 숫자 3개**로 이루어진 **3자리 수**를 랜덤으로 생성합니다.
- 플레이어는 3자리 숫자를 입력해 정답을 추리합니다.
- 입력 후 결과는 다음과 같은 방식으로 제공됩니다:
  - `S (Strike)` : 숫자와 자리까지 맞은 경우
  - `B (Ball)` : 숫자는 맞지만 위치가 다른 경우

<br>

### 📚 02. 예시
| 정답 | 플레이어 입력 | 결과   | 설명                                        |
|-----|--------------|--------|---------------------------------------------|
| 427 | 123          | 0B 1S  | `2`가 자리까지 맞아 1스트라이크               |
| 427 | 247          | 2B 1S  | `4`는 자리까지 맞고, `2`, `7`은 위치만 다름      |
| 427 | 589          | 0B 0S  | 일치하는 숫자가 하나도 없음                   |

<br>

### 🏆 03. 승리 조건
- `3스트라이크`를 기록하면 게임에서 승리합니다!

<br>

# 🛠 개발 과정
- Unreal Engine과 C++로 구현해 보았습니다.

<br>

### 📌 01. 개발 목표
- Unreal Engine C++ 기반의 채팅 프로젝트 구조 설계 연습
- Replication, RPC, Event Dispatcher 등 언리얼 네트워크 기능 적용

<br>

### 🚀 02. 프로그램 흐름도

<img src="https://github.com/user-attachments/assets/da235b96-66d3-4b0d-a05f-78f593d4ebeb" width="800" height="800"/>


### 03. 기능 구현
✅ Listen Server 기반 설계
- 클라이언트 중 한 명이 서버 역할을 수행
- Server, Client, Multicast RPC로 권한과 네트워크 흐름 분리
- 모든 게임 진행 로직과 판정은 서버(GameMode) 에서 수행

✅ 랜덤 숫자 생성
- 클라이언트 접속시 게임이 시작되며 랜던한 숫자 생성

✅ 입력 및 판정 시스템
- 플레이어의 입력값 검증
- Strike / Ball / Win 판정 로직 구현

✅ 네트워크 구조 및 핵심 기술
- 클라이언트 서버 로그인 (`UFUNCTION(Server, Reliable)`)
- 클라이언트 채팅 전송 (`UFUNCTION(NetMulticast)`)
- 서버 판정 결과 전송

✅ UI/UX
- `UMG` 위젯으로 숫자 입력 / 판정 결과 출력
- `Event Dispatcher` 기반으로 HUD 연동 처리
- 시도 횟수, 남은 기회 등 시각화
