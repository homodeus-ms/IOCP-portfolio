**플레이 영상**

[Watch this video](https://youtu.be/hjL432mWlBg)

IOCP서버 구현 + 간단한 2D 로그라이크 게임 온라인 연동 포트폴리오입니다.

Winapi와 C++로 구현되어 있습니다.

크게 IOCP ServerCore / Server / Client 세 파트로 나눠져 있습니다.

<br>

**현재까지 구현된 기능은 대략 이렇습니다.**
1. IOCP서버를 이용해서 대량의 클라이언트 접속을 받을 수 있습니다.
2. 플레이어는 방향키로 이동을 하고 스페이스키로 공격을 합니다.
3. 오브젝트들이 이동할 때 Flipbook 애니메이션 효과가 적용됩니다.
4. 타일맵을 이용해서 갈 수 있는 곳과 갈 수 없는 곳이 구분됩니다.
5. 숫자키 1(칼), 2(화살)로 무기를 변경할 수 있습니다. 뱀은 칼로는 1방에 죽고 화살로는 3방에 죽습니다.
6. 등장하는 몬스터는 뱀 한 종류이고 가까운 플레이어를 찾아서 이동하고 거리1에서 공격을 합니다.  
   단, 플레이어에게 입히는 데미지는 구현하지 않았고 공격 이펙트만 뜹니다.
7. 뱀은 랜덤 위치에 50마리가 등장하고, 일정 수 이하가 되면 리스폰 됩니다.
8. 간단한 채팅을 할 수 있습니다.
9. 클라이언트에서 선 이동 후 서버에 보고하는 방식을 채택했습니다.

<br>
<br>
<br>


**각 파트에 대한 대략적인 설명입니다**

***
**ServerCore**

1. Network폴더에 IOCP관련 핵심 기능들이 분산 구현되어 있습니다.

2. Thread폴더의 Lock파일에 Reader-Writer Lock이 구현되어 있습니다.
   * Write를 할 때만 배타적으로 동작, Read할 때는 자원을 공유합니다.
   * 재귀적으로 Lock을 획득할 수 있습니다.

3. Memory폴더에 Pooling 기능이 구현되어 있습니다.
   * MemoryPooling과 ObjectPooling
   * MemoryPool는 내부적으로 LockFreeStack을 사용하기 위해서 마이크로 소프트 SList가 사용되었습니다.
   * Allocator파일에는 PoolAllocator, StlAllocator가 구현되어 있습니다.
   * 풀링 기능을 뒤늦게 붙이기도 했고 모든 곳에 풀링을 사용하지는 않았습니다.

4. Job폴더에 Job기능이 구현되어 있습니다.
   * JobQueue에서는 내부적으로 LockQueue가 사용되었고 LockQueue는 Utils폴더에 구현되어 있습니다.
   * TLS를 통해 현재 쓰레드가 맡고 있는 Job이 있는 지 여부와 지금까지의 진행 시간을 체크해서  
     일감을 분배 실행하고 있습니다.
   * 만약 벌써 맡고 있는일이 있거나, 제한 시간이 지난 경우에 GlobalQueue에 일감을 밀어 넣습니다.
  
5. Main에는 pch, Global변수들, Defines, TLS, Defines, Types와 같은 헤더들이 존재합니다.

6. 이것은 라이브러리 형태로 컴파일되어서 Server와 Client쪽에서 사용할 수 있는 기능을 제공합니다.

<br>

***

**Server**

1. 클라이언트와의 통신은 ProtoBuf를 사용했습니다.

2. server.cpp 파일이 진입점입니다.

3. Main폴더의 ServerPacketHandler와 GameSession을 통해서 네트워크 작업이 진행됩니다.
   * GameSession은 ServerCore의 PacketSession클래스를 상속 받습니다.  
     부모 클래스의 OnRecvPacket()과 같은 함수들을 오버라이드해서 흐름을 가져옵니다.
   * ServerPacketHandler클래스의 MakeSendBuffer부분은 자동화 할 수 있겠지만  
     ProtoBuf방식도 생소했고 전체 구조를 좀 더 가독성있게 보고 싶어서 굳이 그렇게 하지 않았습니다.

4. Game폴더의 Object파일에는 현재 게임에 등장하는 오브젝트들의 클래스가 있습니다.
   * Player / Monster(snake) / Projectile(Arrow) 입니다.
   * Monster는 일정 범위에 들어온 가장 가까운 플레이어를 찾아 그쪽으로 이동합니다.  
     A*로 구현되어 있고, 실제 코드 구현은 GameRoom.cpp에 있습니다.  
     맵이 열릴때 랜덤위치에 50마리까지 생성되고, 수가 일정 수 이하로 감소하면 리스폰됩니다.
   * Projectile(Arrow)는 일정 거리 이상 이동하거나 물체에 맞으면 자동으로 소멸합니다.
   * ProtoBuf를 사용했기 때문에 GameObject클래스 같은 곳에서 Object의 info같은 것들을  
     Protocol::ObjectInfo _info 와 같은 형태로 멤버변수로 그대로 들고 사용하고 있습니다.

5. Room폴더에는 게임 전체를 관장하는 Room과 TileMap클래스가 있습니다.
   * TileMap을 사용해서 이동 관련 처리를 했습니다.
   * GameRoom.cpp 파일에 전달된 패킷의 종류에 따라 패킷 검증 후 실제로 처리하는 로직들이 들어있습니다.
   * _trashIds와 RemoveTrash() 와 같은 기능은 오브젝트 풀링기능을 넣기 전에 소멸되는 오브젝트들  
     (특히 화살)의 소멸 타이밍을 컨트롤 하기 위해서 만든 기능입니다.  
     일정 갯수 이상 모아두었다가 한꺼번에 지우는 방식으로 구현했습니다.

6. 가장 어려움을 겪었던 부분은 역시 네트워크 패킷들을 '잘 처리'하는 부분이었습니다.  
   클라이언트 화면과 서버의 데이터의 동기화 작업이 생각보다 어려웠습니다.  
   열심히 디버깅해서 현재까지 구현된 것들의 기능적인 부분은 문제가 없다고 생각하지만,  
   접속한 친구의 움직임이 가끔 부자연스럽게 튀는 현상이 있어서 고민중입니다. (첨부한 영상 말미에 보입니다.)



<br>

***

**Client**

1. 최대한 프레임워크를 가지는 형태로 구현되어 있습니다.
   * 각 폴더에 엔진코드와 컨텐츠 코드(Game)로 분리를 해보려고 했습니다.

2. Client에서 네트워크 처리를 하는 부분은 Network폴더의 클래스들과 Manager폴더의 NetworkManager입니다.  
   Server와는 다르게 Client는 하나의 쓰레드만 돌아갑니다.

3. 전체적으로 Unreal방식의 Init/Update/Render 함수명과 Unity방식의 BeginPlay/Tick/Render 함수명이 혼용되어 있습니다.

4. GameObject들은 기본적으로 FlipbookActor클래스를 상속받아서 Flipbook 애니메이션이 적용되어 있습니다.

5. 현재까지 만들어진 Scene은 DevScene한개로 DevScene에 많은 기능들이 들어있습니다.  
   Server에서의 Room과 Client에서의 DevScene의 역할이 비슷 한 것 같습니다.

5. Chat 기능은 WndProc의 흐름을 가로채와서 Game.cpp->InputManager->ChatBox로 이어집니다.
   * 기본적으로 입력하는 글자를 보여주고 엔터를 치면 전송되어서  
     가장 최근에 입력된 chat을 10개까지만 보여주는 형태입니다. 
   * Chat은 네트워크 통신을 테스트하는 느낌으로 간단하게 구현해서 완전하진 않습니다.
   * 그리 길지 않은 문장들을 받는다는 전제로 만들어져 있습니다.
   * 아주 긴 문장이 들어온다면 비록 std::string으로 받고 있어서 메모리 문제가 발생하진 않겠지만  
     내부적으로 계속 할당공간을 늘리고 복사 할 것이기에 비효율적일 것입니다.  
     그리고 현재는 문장이 아주 길다면 화면에 보여지는 것도 잘리게 되는 상태입니다.

6. Component 폴더의 CameraComponent는 플레이어에 부착되어 사용되고 있지만  
   Collider는 충돌을 공부하는 용도로만 만들어져 있습니다.  
   결국 Tilemap방식이 채택되어 현재 Collider기능은 사용하지 않고 있습니다.  

<br>
<br>


**참고한 것들, 사용된 Source**

1. 전반적인 구조와 코드 구현의 많은 부분을 인프런 Rookiss님 강의를 바탕으로 공부했고 참고했습니다.
   
2. 젤다 이미지와 뱀 이미지역시 강의에서 얻게 된 이미지입니다.

3. 맵 이미지는 kenney_urban_pack을 사용해서 Tiled라는 프로그램으로 직접 만들었습니다.

4. 현재 게임에 사용된 배경음악은 Wedance의 '숨'이고 저작권 문제는 없습니다.


<br>
