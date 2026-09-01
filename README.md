# Vanguard

> 좌우로만 움직이며 밀려오는 적을 막아내는 **레인 기반 오토슈터**

**Unreal Engine 5.8** · **C++** · 1인 개발 (2026.07 ~ )

<!-- TODO: 플레이 영상 GIF 삽입 -->
<!-- ![gameplay](docs/gameplay.gif) -->

---

## 게임 소개

플레이어는 하나의 레인 위에서 **좌우로만** 이동합니다. 전진도, 조준도, 발사 버튼도 없습니다. 장착한 무기는 알아서 발사되고, 적은 정면에서 계속 밀려옵니다.

플레이어가 내리는 결정은 두 가지뿐입니다.

- **어디에 서 있을 것인가** — 적의 공격 사거리 밖에 머물면서도 사격 각을 확보해야 합니다.
- **어떤 무기를 들 것인가** — 무기마다 무게가 다르고, 무거운 무기를 들수록 느려집니다. 화력과 기동성을 맞바꾸는 선택입니다.

무기는 웨이브마다 등장하는 **무기 박스**를 직접 쏴서 부수면 획득합니다. 박스를 부수는 동안에도 적은 계속 다가오기 때문에, 무기 교체 자체가 리스크를 감수하는 행동이 됩니다.

## 조작

| 입력 | 동작 |
|---|---|
| `A` / `D`, 좌우 방향키 | 레인 위 좌우 이동 |
| 마우스 | 카메라 회전 |
| — | **발사는 자동** (조준·발사 입력 없음) |

Enhanced Input 기반이며, 전진 입력은 코드에서 의도적으로 무시해 레인 제약을 만듭니다.

## 게임 흐름

```
인트로 화면
   ↓
STAGE N 배너 (1.5초)
   ↓
웨이브 진행 ──→ 적 스폰 + 무기 박스 배치
   ↓              (모든 적 처치 시 다음 웨이브)
STAGE CLEAR 배너
   ↓
   ├─ 다음 스테이지 → 반복
   ├─ 전 스테이지 클리어 → ALL CLEAR (도달 스테이지 · 처치 수 표시)
   └─ 플레이어 사망 → GAME OVER (도달 스테이지 · 처치 수 표시)
```

## 무기

| 무기 | 발사 간격 | 특징 |
|---|---|---|
| **Gun** | 0.2초 | 기본 무기. 단발 직선 탄환 |
| **Gatling** | 0.1초 | 총열을 순환하며 발사. 최고 연사 대신 무게가 무거움 |
| **Grenade** | 1.0초 | 착탄 시 범위 폭발. 중심에서 멀수록 데미지 감쇠 |
| **Sniper** | 1.0초 | 저연사 고화력 |
| **Laser** | 상시 | 발사체가 아닌 지속 빔. 일정 주기로 반복 피해 |

무기 무게는 그대로 이동 속도에 반영됩니다.

```cpp
MaxWalkSpeed = Clamp(BaseWalkSpeed - Weight * 5, 100, BaseWalkSpeed)
```

## 기술적 포인트

### 데이터 주도 스테이지 구성

스테이지 진행은 `AStageManager` 하나가 전담합니다. 진행 데이터는 C++에 하드코딩하지 않고 중첩 구조체로 정의해 두고, 실제 수치는 레벨에 배치된 블루프린트에서 편집합니다.

```
FStage ─ TArray<FWave> ─ TArray<FSpawnEntry>{ 적 클래스, 마리 수, 스폰 간격 }
                       └ 해당 웨이브에 배치할 무기 박스 / 무기 클래스
```

스폰 진행 상태는 `SpawnEntries`와 인덱스가 일대일로 대응하는 별도 배열에 누적합니다. 덕분에 데이터(무엇을 스폰할지)와 런타임 상태(얼마나 스폰했는지)가 섞이지 않고, **새 스테이지를 추가하는 데 코드 수정이 전혀 필요 없습니다.**

### 인터페이스로 통일한 데미지 경로

데미지를 받는 모든 대상은 `IDamageable` 하나만 구현합니다.

```cpp
class IDamageable
{
public:
    virtual void TakeDamageAmount(float DamageAmount) = 0;
};
```

탄환 오버랩, 수류탄 스플래시, 레이저 지속 피해가 전부 `Cast<IDamageable>` 한 줄을 거칩니다. 그래서 적과 무기 박스처럼 성격이 전혀 다른 액터도 발사체 쪽 코드를 건드리지 않고 똑같이 피해를 받습니다. 새 발사체나 새 피격 대상을 추가할 때 기존 코드를 수정할 일이 없습니다.

### 델리게이트 기반 사망 통지

어떤 액터도 다른 액터의 상태를 매 프레임 검사하지 않습니다. 사망은 다이나믹 멀티캐스트 델리게이트로 위쪽에 알립니다.

- `AEnemy::OnEnemyDied` → `AStageManager`가 생존 수를 줄이고 처치 수를 누적
- `AVanguardCharacter::OnCharacterDied` → `AStageManager`가 게임 오버 처리

스테이지 매니저는 적을 순회하지 않고, 적은 매니저의 존재를 모릅니다.

### 반복 순회를 피한 무기 박스 관리

무기 박스는 서로 겹치지 않도록 앞선 박스와의 간격을 유지하며 전진합니다. 이때 다른 박스의 위치를 알아야 하는데, `TActorIterator`는 월드의 전체 액터 배열을 복사하므로 매 프레임 쓰기에 부담이 큽니다.

대신 정적 약참조 배열을 두고 `BeginPlay`에서 등록, `EndPlay`에서 제거하는 방식으로 살아있는 박스만 추적합니다. `TWeakObjectPtr`를 써서 파괴된 박스가 남아도 댕글링 참조가 되지 않습니다.

### 발사체마다 다른 이동 방식

`ProjectileMovementComponent`를 쓰지 않고 각 발사체가 자신에게 맞는 방식으로 움직입니다.

- **일반 탄환** — `Tick`에서 전방으로 직접 이동, 첫 유효 충돌에 소멸
- **레이저** — 이동하지 않음. 매 프레임 라인 트레이스로 총구부터 피격 지점까지 빔 메시를 다시 늘려 붙이고, 타이머로 주기적 피해를 준다. 조준 대상이 바뀌면 피해 타이머를 초기화

### C++ / 블루프린트 역할 분리

모든 C++ 클래스는 블루프린트로 상속해 사용합니다. **C++은 로직과 구조**를, **블루프린트는 수치와 에셋**을 담당하며, C++ 코드가 콘텐츠 경로를 직접 참조하는 곳은 없습니다. 밸런스 수정에 재컴파일이 필요하지 않습니다.

## 클래스 구조

```
Source/Vanguard/
├─ StageManager.h/cpp        스테이지·웨이브 진행 (게임 진행의 단일 주체)
├─ EnemySpawner.h/cpp        스폰 위치 계산 전담
├─ WeaponBox.h/cpp           파괴 시 무기를 지급하는 픽업
├─ Damageable.h              피격 대상 인터페이스
├─ AnimNotify_Attack.h/cpp   공격 몽타주 타격 프레임 통지
├─ Characters/
│  ├─ VanguardCharacter      레인 이동, 무기 장착, 체력
│  └─ Enemy                  추격·공격 AI, 사망 처리
├─ Weapons/
│  ├─ BaseWeapon             자동 발사 루프와 무게
│  └─ Gun / Gatling / Grenade / Laser / Sniper
├─ Projectiles/
│  ├─ BaseProjectile         직선 이동과 오버랩 피해
│  └─ Bullet / Grenade / Laser
└─ Widgets/
   ├─ HealthBarWidget        월드·스크린 양쪽에서 쓰는 체력 바
   ├─ StageBannerWidget      STAGE N / STAGE CLEAR 배너
   └─ EndScreenWidget        게임 오버·올 클리어 결과 화면
```

## 빌드 / 실행

Unreal Engine 5.8 설치가 필요합니다.

```powershell
# 에디터 타겟 빌드
& "C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" `
    VanguardEditor Win64 Development -Project="<경로>\Vanguard.uproject" -WaitMutex

# 에디터 실행
& "C:\Program Files\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe" "<경로>\Vanguard.uproject"
```

`.uasset` / `.umap`은 Git LFS로 관리하므로 클론 전에 `git lfs install`이 필요합니다.
