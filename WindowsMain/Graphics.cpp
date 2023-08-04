/* 좌표계( Coordinate System )및 기하학( Geometry ) */

/*
x 축 y축으로 되어 있던 평면은 르네 데카르트가 고안한 x-y평면 기하학에서 출발했다.
이러한 공간을 데카르트 좌표계라 부르며 X축 Y축이 교차하는 형태를 따서 직교 좌표계라 부른다.

도형은 표시하기 위한 최소 단위는 점이다. 이러한 점은 그 기준이 되는 축으로 수직으로 연결한 값을 좌표로 가지게 된다.
그래서 직교 좌표계라 부른다.

여기에 z축을 하나 추가하면 3차원 공간이 가능해 진다. x-y 평면, x-z 평면, y-z 평면 으로 이루어지는 이 좌표계는
z축의 방향에 따라 다음과 같이 두 가지로 나눠진다.

LEFT-HANDED Cartesian Coordinates
Y    Z
|   /
|  /
|ㅡㅡㅡㅡ X

RIGHT-HANDED Cartesian Coordinates
	Y
	|
	|
	|ㅡㅡㅡㅡ X
   -
  -
-
Z
손가락의 엄지 손가락 방향으로 z축이 증가한다라는 의미로 각각 왼손 좌표계, 오른손 좌표계라 부른다.
DirectX는 왼손 좌표계를 사용하며 OpenGL은 오른손 좌표계를 사용한다. 유니티나 언리언 엔진 역시 왼손좌표계를 사용한다.
( 축의 방향이 다른 경우도 있으며, 내부적으로는 더 다양한 좌표계가 사용된다. )
*/

/* Vertex & Vector */

/*
좌표계를 사용하면 공간에 꼭짓점( 정점, 단수 Vertex, 복수 Vertices )과 방향( Vector )을 지정할 수 있다. 여러 개의 버텍스를 사용
하면 선이나 도형을 만들 수 있게 된다. 이렇게 만들 수 있는 도형들 중 가장 기본 형태를 Primitives( 원형 )라 부르며 수 많은 Primitives
들로 이루어진 복잡한 도형을 메시( Mesh )라 부른다.

Mesh 라는 단어의 뜻이 그물망, 망사라는 뜻인데 3차원으로 표현한 도형의 형태가 Mesh 처럼 생겼기 때문이다.
폴리곤( Polygon, 다각형 )이라는 것이 있다. 이 폴리곤은 Primitives를 의미하는데, 초창기 가장 많이 사용된 오브젝트를 표현하는 가장 최소 단위
가 삼각형이었고 이를 폴리곤이라 불렀기 때문에 일반적으로 통용되고 있을 뿐이다. 최근에는 사각형을 Primitives로 사용하는 3D 편집
프로그램도 있기도 해서 기존의 삼각형 폴리곤과 구분하기 위해 Primitives라는 단어를 더 많이 사용하려고 하는 것 같다.

기본 요소들로 도형을 표현하는 방법을 저체적으로 살펴 보면 다음과 같다.

Vertices( 꼭짓점 ) : 도형을 구성하는 각 꼭짓점들의 정보( 도형의 기본 정보 )
Edges( 모서리 ) : 두 버텍스를 연결하면 모서리가 만들어 진다. 주어진 버텍스들로 만들 수 있는 모서리의 경우의 수는 매우 다양하므로
	모서리 정보가 매우 중요하다.
Faces( 면 ) : 엣지들을 연결하면 형태를 가지는 평면이 만들어 지는데, 3차원 공간이므로 그 평면이 어느쪽을 바라 보는지가 중요해 진다.
	이런 방향성을 지정하는 것이 Face
Polygons : Face가 지정되면 도형을 이루는 최소 단위인 폴리곤이 만들어 진다.
Surfaces : 동일한 특성을 가지는 폴리곤들을 묶어서 집합을 만들어 서피스라 부른다.

기본적으로 지금까지 경험한 3D 게임들은 모두 표면(Surface)를 표현한 것들 뿐이다. 3차원 오브젝트의 내적인 표현은 아직까지 개념이 없다.
*/

/* 법선 벡터( Normal Vector ) */

/*
메시의 각 페이스에 해당 페이스가 바라보는 방향을 나타내는 정보가 필요하다.
3차원 렌더링을 효율적으로 처리하기 위해 뒷면은 그리지 않는 방식을 사용하기 때문에 해당 정보는 매우 중요 하다.

이러한 정보를 법선 벡터라 부르며 해당 면에 수직인 단위 벡터( 길이가 1인 벡터 )를 사용한다. 이런 정보는 좌표계에 따라 정의 되며
왼손 좌표계에서는 버텍스의 시계 방향 순서로 결정된다.

그리고 버텍스에서 법선벡터가 존재하는데, 이 정보를 사용하여 음영처리를 할 수 있다.
*/

/* Transform */

/*
Direct2D 연슬할 때, 간약하게 이동, 회전, 크기에 대해 살펴 봤다. 3차원 공간의 버텍스를 모니터에 표현하기 위해선 매우 복잡한 변형이 이루어 진다.
각 변환들은 간단한 수학 공식으로 증명이 가능하다.

Model Space : 3D 오브젝트는 자체적인 원점을 가진 좌표계를 가지고 있다.
	보통 아티스트는 캐릭터를 원점 기준으로 모델링을 하고 리소스를 제공해 준다. 이러한 좌표가 로컬 좌표계라 하며 모델 공간이라고도
	한다. 이 단계에서는 변환 없이 디자인한 대로 버텍스들이 저장되어 있다.

World Transform : 게임에서는 다양한 오브젝트들을 배치해서 화면을 구성한다. 이 때 기준이 되는 좌표가 월드 좌표계이다.
	캐릭터를 월드에 배치하려면 위치를 지정하고 적당히 회전하고 크기도 조절해야 한다. 즉, 모델 공간에서 월드 공간으로 변환이 일어
	나는 것이다. 이 과정을 World Transform이라 부른다.

View Transform : 3D 환경은 표현을 위해 카메라를 사용하며, 그 카메라에 찍힌( 투영된 )이미지를 화면에 표시한다.
	이를 위해서 수행되는 변환을 View Transform 혹은 Camera Transform이라 한다. 이 역시 단순한 행렬의 곱셈일 뿐이다.
	카메라를 원점으로 하는 좌표계에 각 버텍스를 변환해서 배치한다고 생각하면 된다.

Projection Transform : 지금까지는 3차원 공간에서 점을 이리 저리 움직이는 과정일 뿐이었다. 하지만 모니터에 3차원 공간을 표현하기 위해서는
	투영이라는 과정이 필요하다. 흔히 미술 시간에 배운 원근법을 적용하거나 하는 기법을 사용한다.
	이러한 과정 역시 수학으로 표현이 가능하며, 이 과정을 Projection Transform이라 부른다.
	이 과정을 거치고 나면 3차원 공간의 버텍스들이 2차원 공간으로 투영된다.

Clipping ScreenSpace : 마지막으로 수행하는 과정이 모니터에 그리는 과정이다. 화면에 배치된 3D 모델에 색상을 입히고 셰이더를
	적용하고 특수효과를 입히는 마무리 단계이다.
	이 때 중요한 것은 화면에 보이지 않는 점들은 이런 복잡한 연산을 수행할 필요가 없다.
	3D 그래픽은 비주얼도 비주얼이지만 성능이 최우선이므로 불필요한 계산은 최대한 배제하는 것이 좋다.
	화면 공간으로 잘라 주는 작업을 Clipping이라 부른다.
	이렇게 잘라진 공간의 최종 버텍스들만 텍스쳐를 입히거나 음영 처리를 한다.
*/

/* Device */

/*
한 장면을 구성하기 위해서는 매우 많은 수하적 연산이 필요하다. 과거에는 이들을 전부 CPU로 처리했었다. 게임 관련 로직도 처리해야 하고
3D 그리팩도 표현해야 했으니 당연히 매우 거친 화면이 될 수 밖에 없었다. 하지만 3D 가속 그래픽 카드의 등장으로 이런 복잡한 연산들은
모두 그래픽카드에게 맡길 수 있게 되었다.
				< Direct3D Device Architecture >
	[Transform Module]	[Lighting Module]	[Rasterizer]
디바이스는 다음과 같은 기능을 포함하고 있다.
	Transform Module : 기하학 변환에 관련된 벡터, 행렬의 연산을 수행한다.
	Lighting Module : 조명 처리에 관련된 연산을 수행한다. 조명에 대한 연산은 광원과 버텍스의 노멀벡터들과의 상대 위치 및
		방향에 대한 계산 결과로 보간하는 형태로 수행된다.
	Rasterizer : 래스터화라는 개념은 모니터의 최소단위가 화소라 발생하는 문제를 해결하기 위한 방법이다.
DirectX에 지원하는 장치 유형은 다음과 같은 두 가지가 있다.
	HAL Device
	하드웨어 추상화 계층 디바이스는 DirectX의 기본 디바이스 유형으로, 위에서 언급한 변환/조명/래스터화 모듈의 전부 또는 일부를 하드웨어에서 구현하게 된다.
		개발자들은 직접 그래픽카드에 접근할 수 없으며, 개발자들은 단지 DirectX의 함수를 호출하며 DirectX가 HAL을 통해 그래픽카드
		에 접근한다. 이런 방식을 사용하고 있는 이유는 하드웨어 제조사별로 자신들만의 노하우로 그래픽카드를 만들기 때문에 내부 작동
		방식이나 사양이 달라 질 수 있다. 이런 서로 다른 기능들을 게임 개발자가 모두 익히는 것은 불가능 하기 때문에 DirectX에서
		표준 인터페이스를 만들고 하드웨어 레벨의 기능은 추상화 시켜 둔 것이다.
	Reference Device
	하드웨어 추상화 계층을 사용하게 되면 개발자는 많은 정보를 얻을 수 없다. 그래픽 제조사 마다 결과 값을 넘겨 주는 방식도
		다르고 결과값 마저도 다르기 때문에 독자적인 디버깅 툴을 제공하기도 한다.
		이 문제를 해결하기 위해 DirectX에서는 모든 모듈의 기능을 지원하는 함조 장치를 추가 했다.
		참조 장치라고 부르는 이유는 이 장치는 디버깅을 위한 특수한 목적으로 탄생되었기 때문이다. 속도 보다는 정확도를 위해
		구현되었고, 소프트웨어에서 구현되기 때문에 기능 테스트를 위해 사용할 수 있다.

Device Lost
게임을 플레이 하다보면 Alt + Tab으로 작업을 전환하거나 절전 모드가 작동하거나 다른 게임을 실행할 경우 비정상적으로 종료되는 경우가 있다.
3D 그래픽을 표현하기 위해서는 상당히 많은 하드웨어 자원을 사용해야 하는데 ,이 자원들에 다른 앱에 의한 손상이 발생할 가능성이 높기
때문에 DirectX에서는 장치의 상태를 작동 or 손상 상태로 구분한다. 이렇게 장치가 분실되면 모든 비디오 메모리를 해제하고 재설정하거난 최악의
경우에는 사용이 가능한 다른 장치를 선택해야만 한다.
이 과정이 잘못되면, 앱이 응답하지 않는 결과가 발생하므로 상용 게임을 제잘할 때는 주의해야 할 부분이다. 다행히 최신 DirectX에서는
이런 빈도수가 많이 줄어서 거의 발생하지 않는다만, 물리적으로 장치가 제거되거나 절전 모드에 들어갈 때는 여전히 주의해야 한다.
*/

/* Swap Chain */

/*
부드러운 애니메이션을 위해 여러 개의 버퍼를 사용하는 버퍼링, 이런 버퍼들의 집합을 Swap Chain이라 부르며 이 과정을 Flip 혹은 Swap
이라 부른다. 화면에 3D 장면을 그리는 과정에는 컴퓨터의 성능도 중요하지만 모니터 역시 중요한 요소다. 모니터는 초당
60 ~ 100회 정도 새로고침을 수행하는데 이건 컴퓨터의 처리 속도보다 현저하게 느린 속도다.
게임을 하다보면 테어링이라는 단어를 듣게 된다. 모니터의 화면 갱신 주기와 우리가 만든 3D 화면의 갱신 주기가 서로 충돌을 일으키게
되면 테어링이라는 증상이 일어나게 되는 것이다.

즉, 버퍼를 교환하는 중에 모니터 갱신이 발생하면 교환 중이던 2개의 이미지가 동시에 표현되면서 이런 증상이 생기게 되는 것이다.
그래서 나온 것이 수직 동기화이다. 쉽게 말하면 모니터가 갱신이 현재 갱신 주기를 모두 마칠 때까지 기다렸다가 화면에 그리는 것이다.
물론, 그릴 때 갱신 주기를 놓치면 다음 번 까지 기다려야 하므로 1/60초를 그냥 허비하게 되는 셈으로 입력 랙과 같은 증상이 발생하기도 한다.
*/

/* Lighting */

/*
3D 장면을 구성하는 핵심 요소 중 하나는 조명 이다. 조명은 단순히 밝기만을 제어하는 것이 아니라 위와 같이 반사, 난반사를 포함하여
재질에도 영향을 주는 복잡한 계산을 필요로 한다.

초기 그래픽카드들은 이런 기능을 강조하기 위해 T&L 가속이라는 용어를 사용할 정도로 조명은 중요하고 복잡한 연산이 필요하다.
가장 기본적인 조명에는 지향광과 주변광이 있다. 둘 다 씬에 배치된 물체에 사물을 비추는 용도로 사용되지만 각각 특성이 있다.

Direct Light(Directional Light) : 직접 오브젝트를 비추며 항상 방향과 색이 있다. 방향이 존재하기 때문에 페이스의 노멀과 연산을 통해
밝기를 정할 수 있다.
Ambient Light : 장면의 모든 오브젝트에 영향을 주는 빛으로 종류, 위치와 상관없이 전체를 채우는 일정한 감도를 가진다. 흔히 태양광으로 사용된다.

Ray-Tracing
기존의 방법들은 화면에 그릴 버텍스에 대한 광원의 위치, 각도등을 계산하는 방식이다. 하드웨어의 성능이 높아지면서 조금 더 그럴듯한
장면을 그리기 위해 사용되는 기법이 광선 추적이다.

화면에 표시되는 점에서 강에서 가상의 빛이 발사된다고 가정하고 그 빛이 진행하는 방향을 역으로 추적하는 방식이다. 위 이미지 처럼 하나의 픽셀에
수많은 연산 처리가 들어가기 때문에 매우 복잡하고 계산이 어렵다.

Ray_Tracing 기법을 적용한 언리얼 엔진 스크린샷이다. 복잡한 계산인 만큼 실사와 매우 유사한 결과물을 그릴 수 있게 된다.
다만 이 기법은 DirectX 기본 지원이 아니기 때문에 직접 구현해야 하며 하드웨어 가속의 지원이 없으면 연산 부하가 커서 게임에 사용하기 힘들다.
*/

/* Shading */

/*
그림자를 처리하는 셰이딩은 3D 객체를 표현하기 위한 필수적인 기법이다.
Flat Shading : 가장 간단한 방식으로 면의 법선과 광원과의 방향으로 음영을 칠하는 방법이다.
Gouraud Shading : 헨리 고러드가 고안한 보간 방법으로 가장 많이 사용되는 방식이다. 각 버텍스의 법선벡터로 평균을 구한 다음 가중치를
생성한다. 이 가중치들을 쌍선형보간하여 부드러운 색상들을 만들어 낸다.
Phong Shading : 그래픽스에 큰 역할을 한 베트남 출신의 연구자 부이 투옹 퐁 이 개발한 방식이다. 각 면의 법선 벡터에 따라 조명에 대한
연산을 보간하는 방식으로 당시에는 매우 획기적인 아이디어였지만 요즘은 표준 방식이다. 렌더링된 픽셀 당 계산을 수행하는 방식이라 픽셀
셰이더의 등장과 함께 대세가 되었다.
*/

/* Depth Buffer & Stancil Buffer */

/*
3차원 공간의 오브젝트를 프로젝션 공간으로 매핑할 때 주의해야 할 점은 어떤 오브젝트가앞에 있는지를 파악하는 것이다.
이렇게 깊이를 관리하기 위해 별도의 버퍼를 사용하는데, 이를 깊이 버퍼라 하며 z값을 사용한다고 하여 z-buffer혹은 w-buffer라 부르기도 한다.
*/

/* Texture */

/*
텍스쳐는 질감이나 감촉을 의미하는 단어로 3차원 오브젝트의 표면에 세부적인 표현을 위해 색을 칠하고나 색상을 지정하는 것을 의미한다.
가장 기본적인 방법은 2D이미지를 사용해 3D 오브젝트에 상세적인 표현을 하는 것이다. 즉, 컴퓨터 그래픽에서의 텍스쳐는 개체에 질감을
더하는 픽셀의 비트맵이라 볼 수 있다.
둥근 지구본을 평면으로 펼쳐서 지도를 제작하는 과정과 반대의 과정으로 2차원의 이미지를 3차원 오브젝트에 적용하는 방식을 사용한다.
Texture Map을 사용하여 3D 오브젝트의 면에 적용하는 방식으로 매핑이라 부른다.

텍스쳐의 가장 기본적인 단위를 Texel이라 부르며 Texture Element 혹은 Texture Pixel을 의미한다. 테셀의 집합( 텍스쳐 ) 역시 좌표계가
존재하며 다음과 같은 형태가 된다.

좌표계의 가로/세로는 U/V축이며 0 ~ 1로 단위화되어 있는 공간이다. 따라서 텍스쳐 매핑은 종종 UV 매핑이라고도 부른다.
*/

/* 텍스쳐 필터링( Texture Filtering ) */

/*
3D 개체는 특성상 화면에 매우 가깝거나 매우 멀게 표현될 때가 있다. 그렇게 되면 텍셀 하나가 엄청 넓은 픽셀로 매핑되거나 혹은 수 많은
덱셀이 하나의 픽셀로 매핑되는 경우가 발생한다. 쉽게 말하자면 텍스쳐 비트맵이 엄청 확대되거나 축소된다는 뜻이다.

따라서 그럴듯하게 3D개체를 표현하기 위해서는 텍셀 색상을 혼합하여 픽셀에 알맞는 생상을 찾아야 하는데 이런 복잡한 과정을
텍스쳐 필터링이라 부른다.

Nearest-Point Sampling
	DirectX에서 기본적으로 수행하는 필더링 방식으로 텍셀의 주소를 가장 가까운 정수에 지정하는 방식을 사용한다. 물론 가장 단순한 방식
	이라 텍스쳐의 크기와 오브젝트의 크기가 같으면 정확한 이미지를 보여주지만 확대/축소되었을 때는 이미지가 흐릿하게 나타날 수도
	있다.
Bilinear Texture Filtering
	선형 텍스쳐 필터링은 계산할 점을 기준으로 선을 그어 양 끝의 값에 대한 중간값을 취하는 방식을 말한다.
	대부분의 최신 그래픽 카드는 리니어 필터링에 최적화되어 있다.

Linear Filtering
우선 선형 필터링은 두 지점 사이의 위치를 기준으로 보간을 수행하는 방식을 의미한다.
Bi-Linear Filtering
이중 선형 필터링은 선형 필터링을 2차원으로 확장하여 상/하, 좌/우 4개의 점을 기준으로 값을 계산하는 방식이다.
Tri-Linear Filtering
마찬가지로 삼중 선형 필터링은 3차원으로 확장하여 육면체 8개의 점을 기준으로 값을 계산하는 방식

Anisotropic Texture Filtering
기존의 필터링은 빠른 대신 멀리 있는 텍스쳐는 흐려진다는 단점이 있다. 이런 단점을 해결하기 위해 등장한 개념이 비등방성 필터링이다.

비등방성이라는 뜻은 방향에 따라 물체의 물리적 성질이 다른 것을 말한다. 동일한 금속면이라도 방향에 따라 빛을 반사하는 정도가
다음을 의미하는데, 이런 특성을 사용해서 거리에 따라 텍스쳐 공간을 왜곡 사용하는 필터링 방식이다.

당연히 샘플링할 포인트가 많아지므로 연산 부하가 크지만 아래 그림과 같이 먼 곳도 선명한 표현이 가능하다.

Mip-Maps
복잡한 필터링으로 그럴듯하게 만드는 것보다 거리에 따라 서로 다른 해상도의 텍스쳐를 적용하자는 개념이 Mip-Map이다.
라딘어 Multum In Parvo의 첫 글자를 따서 Mip-map이라 부르는데 개발자의 용어인 피라미드를 여전히 사용하기도 한다.

카메라에서 보여지는 오브젝트의 거리에 따라 서로 다른 텍스쳐를 사용하며 각 단계별로 주로 2의 거듭제곱 크기로 지정한다.
각 단계는 품질의 레벨이라는 용어 Level Of Detail이라 표현한다.
텍스쳐의 크기가 커지긴 하지만 가장 효율적으로 거리에 따른 품질을 유지할 수 있는 방법이다.
*/

/* 파이프라인 */

/*
3차원 그래픽에서 어떻게 그리는 지를 담당하는 것이 렌더링 파이프라인이다.
사전적인 의미로 렌더링은 무언가를 만든다는 뜻이고, 파이프라인은 수송관이라는 뜻이다.
즉, 무엇인가를 만드는 수송관이라는 뜻, 그래서 다음과 같이 생겼다. 각 단계를 Stage라 부른다.

INPUT-ASSEMBER STAGE -> Vertex Shader STAGE -> Hull Shader STAGE -> Tessellator STAGE -> Domain Shader STAGE -> Geometry Shader STAGE -> Rasterizer STAGE -> Pixel Shader STAGE -> OUTPUT MERGER STAGE
																																	  -> STREAM OUTPUT STAGE

INPUT ASSEMBLER : 그리기 위한 모든 입력을 조합한다.

Vertex Shader :
	Vertex : 정점 즉, 모서리의 점( 꼭지점 )이라는 뜻, 3차원 그래픽은 이러한 Vertex로 출발한다.
	Shader : Shade는 음영( 그림자 )을 지게 한다라는 뜻으로 Shader는 그런 음영 처리를 해주는 것이다.
	쉽게 말하자면, 이런 정점들의 위치나 기타 정보들로 음영을 잘 입힐 수 있게 준비해 준다. 하지만, 거의 일반 명사로 자리 잡혀서,
	이제는 그냥 그래픽스와 관련된 처리를 하는 거의 모든 부분을 셰이더라고 부른다.

Hull SHader :
	Hull : 덮개, 껍질이라는 뜻, 선박에서는 선체 측 외곽을 의미하기도 한다.
	간략한 설계도로부터 실제 선체를 다시 만들어 낸다고 생각하면 된다. 간단한 도형( 사이즈가 적은 모델 파일 )에서 보기 좋은 도형
	( 잘게 세분화된 )으로 바꿔주는 역할을 한다.

Tessellator : 분할기라고 하는데 사전적 의미는 격자무늬를 말한다.
	Hull Shader에서 만든 더 세분화된 도형을 삼각형으로 더 잘게 쪼개서 표현한다. 이렇게 자른 삼각형에서 텍스쳐를 매핑하기 위한 정보까지 추출해 낸다.

Domain Shader : 위에서 나온 세분화된 삼각형의 각 꼭지점의 위치를 계산한다.

Geometry Shader : 기하 도형 셰이더이다. 삼각형, 선, 점 들에 대한 계산을 한다.

Rasterizer : 위에서 2차원으로 변환된 위치 정보들을 화면에 표시할 준비를 한다.
	모니터는 격자로 되어 있는데 그 격자에 그림을 그리기 위해 각각의 위치를 만들어 낸다. 그리고 지금까지의 셰이더 정보를 토대로 보여질 그림에 대한 정보를 채운다.

Pixel Shader : 각 픽셀에 저장된 정보( 버텍스 색상, 조명 및 텍스쳐 )를 이용하여 색상을 칠한다. 그리고 보간 등의 기법을 적용해 최종 픽셀을 만들어 낸다.

OUTPUT MERGER : 깊이-스텐실 버퍼 등을 이용하여 지금까지 그린 이미지가 화면에 나타날지, 보일지 말지 등에 대한 처리를 한 후에 최종적으로 화면( 프레임 버퍼 )에 보낸다.
*/

/* HLSL( High Level Shader Language ) */

/*
GPU에서 점점 더 많은 그래픽 처리를 수행함에 따라 이를 프로그래머가 직접 제어할 수 있는 언어가 필요해 졌다. 그 중에서
음영 처리를 수정할 일이 많기 때문에 Shader Language가 만들어 졌고 보편화 되었다.

저수준 셰이더 언어
	당연히 3D 그래픽은 속도가 우선이었으므로 초기에는 기계어와 유사하게 디자인 되었다.
	게다가 PC, Playstation 등 서로 다른 플랫폼에 최적화된 독자적인 셰이더 언어를 사용했기 때문에 개발자들은 익숙하지 않은
	어셈블리를 사용했으며 플랫폼 이식할 때 마다 다른 형태로 작성하는 어려움을 겼었다.
고수준 셰이더 언더
	일반 언어의 발전과 비슷하게 점점 GPU의 성능이 올라가면서 셰이더 언어도 C 스타일이 고급 언어로 발전하게 되었습니다.
	그 중 DirectX에서 사용하는 것을 DirectX HLSL이라 한다. 그 외에도 자주 사용되는 언어는 NVIDIA의 Cg, OpenGL의 GLSL, Apple의 Metal Shading이 있다.
*/

/* DXGI Flip Model */

/*
여러 개의 버퍼를 사용해 자연스러운 애니메이션을 표현하는 방법들 중 가장 자주 사용하는 방법은 Bit Mode( Bit Bit )와 Flip Mode이다.

Bit Mode Present
	DX app surface : DX app Updates its Client area
		DX app surface -> Bit to DWM surface of DX app -> DWM surface of DX app
	Flip Chain Buffer
		DWM renders to screen using DWM's shared DX surface
Flip Mode Present
	DX app surface : DX app updates its client area
		Flip mode present to DWM, no additional bit
	Flip Chain Buffer
		DWM renders to screen using pointer to updated DX surface

DWM : Desktop Window Manager, Windows 7 에서 추가된 윈도우 관리자이다.

Bit Mode :
	앱이 프레임에 그린다.
	D3D가 버퍼의 내용을 다른 버퍼로 복사한다.
	복사가 끝나면 DWM이 화면으로 그린다.
Flip Mode :
	앱이 프레임에 그린다.
	D3D가 버퍼를 DWM으로 넘겨 준다.
	DWM이 화면으로 그린다.

당연히 Flip 방식이 읽기, 쓰기 연산이 빠지므로 빠르다.
과거의 DirectX는 독점 모드에서만 Flip을 지원했는데 Windows7 과 DWM에 의해 창모드에서도 사용이 가능해 졌다.
*/

/* Dirty Rectangle & Scroll Rectangle */

/*
동영상 코덱 등에서는 파일 크기를 줄이기 위해 이전 장면과 현재 장면의 변경된 내용만 그리는 압축을 자주 사용한다.
이렇게 변화해야 할 영역을 갱신이 필요한 더러운 영역이라 부른다.
게임에서도 성능향상을 위해 혹은 동영상을 표현하기 위해 이런 방식을 사용할 수 있다.

Dirty Rect 방식 중 스크롤 형태를 가지는 방식이 있다.

Previous Frame / Back Buffer befor Present -> Present Frame
화면이 스크롤 되어 기존 영역의 위치가 변경되고 일부가 갱신괼 경우 버퍼를 스크롤하고, 일부 영역만 그리는 방식을 사용하면 전체를
갱신하는 것에 비해 효율적으로 화면을 표시할 수 있다.

더블 버퍼를 사용하면 조금 더 깔끔해 진다.
	1. 현재 전면 버퍼의 내용이 화면에 그려진 상태
	2. 후면 버퍼의 dirty rect를 설정해 변경될 영역을 그린다.( 위치나 내용이 변경된 비디오, 새로운 줄 )
	3. IDXGISwapChin::Present를 호출할 때 다음 정보를 넘겨 준다.
		a. Dirty Rectangles : 변경된 영역
		b. Scroll Rectangles : 스크롤 영역
		c. Scroll Offset : 스크롤된 오프셋
	위 정보들로 이전 프레임( 현재 전면 버퍼의 내용 )에서 스크롤 영역을 복사 한다. 물론 dirty rectangle의 내용은 갱신할 테니 가져오지 않는다.
	4. 버퍼 swap 하여 화면에 표시
*/