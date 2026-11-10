# StageCraft

StageCraft는 Unreal Engine 5 C++로 개발 중인 데이터 기반 Stage/Guide Runtime 프로젝트입니다.

Stage의 진행 데이터를 DataAsset으로 정의하고, Step 단위의 Action 실행과 Condition 평가를 통해 다양한 콘텐츠 진행 흐름을 구성합니다.

현재는 서로 다른 두 Level의 Stage를 동일한 Runtime으로 실행하며, 연출·전투·상호작용·보상을 기존 Action / Condition과 콘텐츠 에셋의 조합으로 구성합니다.

> 개발 현황: 5주차 완료 기준. 6주차에는 비동기 실행 수명 관리, Runtime 상태 분리 보완, Debug / Validation 및 최종 포트폴리오 정리를 진행합니다.

> 이 저장소는 StageCraft의 **대표 C++ 소스코드 14개 파일**을 공개합니다. 전체 Unreal Engine 프로젝트 및 에셋을 포함하지 않으므로 이 저장소만으로 빌드하거나 실행할 수 없습니다. 아래 기능 설명은 전체 StageCraft 프로젝트의 5주차 구현을 기준으로 작성했습니다.

## Representative C++ Source Code

포트폴리오에 선정한 14개 대표 파일입니다. 파일명을 클릭하면 해당 소스코드로 이동합니다.

| 구분 | 파일 | 확인할 내용 |
| --- | --- | --- |
| 데이터 정의 | [`StageDataAsset.h`](StageCraft/Source/StageCraft/Stage/Data/StageDataAsset.h), [`StageStepData.h`](StageCraft/Source/StageCraft/Stage/Data/StageStepData.h) | Stage와 Step의 데이터 구성 |
| Action / Condition 공통 기반 | [`StageAction.h`](StageCraft/Source/StageCraft/Stage/Action/StageAction.h) | Action 실행 계약 |
| Condition 공통 기반 | [`StageCondition.h`](StageCraft/Source/StageCraft/Stage/Condition/StageCondition.h) | Condition 평가 계약 |
| Stage 진행 제어 | [`StageInstance.h`](StageCraft/Source/StageCraft/Stage/Runtime/StageInstance.h), [`StageInstance.cpp`](StageCraft/Source/StageCraft/Stage/Runtime/StageInstance.cpp) | Action 실행, Condition 평가, Step 전환 |
| Actor 생성 Action | [`StageAction_SpawnActor.h`](StageCraft/Source/StageCraft/Stage/Action/StageAction_SpawnActor.h), [`StageAction_SpawnActor.cpp`](StageCraft/Source/StageCraft/Stage/Action/StageAction_SpawnActor.cpp) | Actor 생성 기능 |
| 처치 조건 평가 | [`StageCondition_KillTarget.h`](StageCraft/Source/StageCraft/Stage/Condition/StageCondition_KillTarget.h), [`StageCondition_KillTarget.cpp`](StageCraft/Source/StageCraft/Stage/Condition/StageCondition_KillTarget.cpp) | KillTarget 조건 평가 |
| Sequence 연출 Action | [`StageAction_PlaySequence.h`](StageCraft/Source/StageCraft/Stage/Action/StageAction_PlaySequence.h), [`StageAction_PlaySequence.cpp`](StageCraft/Source/StageCraft/Stage/Action/StageAction_PlaySequence.cpp) | 시퀀스 재생 및 완료 이벤트 |
| Stage 전환 | [`StageFlowSubsystem.h`](StageCraft/Source/StageCraft/Stage/Runtime/StageFlowSubsystem.h), [`StageFlowSubsystem.cpp`](StageCraft/Source/StageCraft/Stage/Runtime/StageFlowSubsystem.cpp) | Level 간 Stage 흐름 관리 |

### Recommended Reading Order

1. `StageDataAsset.h` / `StageStepData.h` — Stage 데이터 정의
2. `StageAction.h` / `StageCondition.h` — Action / Condition 공통 계약
3. `StageInstance.h/.cpp` — Stage 진행 제어
4. `StageAction_SpawnActor.h/.cpp` / `StageCondition_KillTarget.h/.cpp` — 확장 기능 예시
5. `StageAction_PlaySequence.h/.cpp` — 비동기 Action 완료 흐름
6. `StageFlowSubsystem.h/.cpp` — Level 간 Stage 전환

대표 파일은 전체 프로젝트에서 발췌한 코드이므로 다른 비공개 헤더·타입을 참조할 수 있습니다.

## Development Environment

- Unreal Engine 5.4.4
- C++
- Visual Studio 2022

## Goals

- Stage 진행 데이터를 DataAsset 기반으로 구성
- Stage 정의 데이터와 Runtime 상태 분리
- Step 단위의 순차적인 콘텐츠 진행
- Action / Condition 확장을 통한 다양한 Stage 구성
- 특정 GameMode에 진행 로직을 작성하지 않는 Stage Runtime 구성
- 동일한 Framework에서 기존 기능과 데이터 조합으로 서로 다른 Stage 콘텐츠 구성
- World 단위의 Stage 실행과 Level 간 Stage 전환 책임 분리

새로운 게임플레이 기능은 Action / Condition 등의 C++ 확장이 필요할 수 있습니다. 데이터 기반 구성의 범위는 이미 구현된 기능과 콘텐츠 에셋을 조합하여 Stage 흐름을 변경하는 것입니다.

## Current Features

### Stage Data

- `UPrimaryDataAsset` 기반 Stage 정의
- `FStageStepData` 기반 Step 구성
- 하나의 Stage에 여러 Step 구성
- Step별 Action / Condition 설정
- Step별 Objective 데이터 구성
  - Objective Text
  - World Marker 표시 여부 및 위치
- `NextStageId`를 통한 다음 Stage 지정
- `UStageCollectionDataAsset`에서 Stage ID와 Stage DataAsset / Level 연결
- DataAsset 구성을 통한 Playable Stage 진행 흐름 구성

### Stage Runtime

- `UStageInstance` 기반 Stage Runtime 상태 관리
- Stage Lifecycle 관리
  - NotStarted
  - Ready
  - Playing
  - Failed
  - Completed
- `Prepare()`와 `Start()`를 분리한 Stage 시작 흐름
- Ready Duration 이후 실제 Gameplay 시작
- Step 순차 진행
- Delegate 기반 Action 순차 실행
- Action 실행 결과에 따른 성공 / 실패 처리
- Condition 평가 결과 구분
  - Satisfied
  - NotSatisfied
  - Invalid
- Runtime 오류 발생 시 Stage Failed 처리
- 마지막 Step 완료 후 Stage 완료 처리

### Action

- `UStageAction` 기반 다형적 Action 구조
- DataAsset에서 Action을 Inline Instance로 구성
- 현재 Action의 완료 이벤트를 받은 후 다음 Action 실행
- 동기 Action과 Timer / Sequence 기반 비동기 Action을 같은 순차 실행 흐름에 연결

현재 구현된 Action:

| Action | 역할 |
| --- | --- |
| `UStageAction_Log` | 로그 출력 |
| `UStageAction_SpawnActor` | World Actor 생성 |
| `UStageAction_ShowDialog` | Dialog 표시 |
| `UStageAction_Reward` | Player Gold 지급 |
| `UStageAction_CameraShake` | Camera Shake 재생 요청 |
| `UStageAction_Delay` | 지정 시간 이후 완료 통지 |
| `UStageAction_PlaySequence` | Level Sequence 재생 및 자연 종료 시 완료 통지 |

Camera Shake 재생과 시간 대기를 별도 Action으로 구성하여 연출과 진행 간격을 조합할 수 있습니다.

### Condition

- `UStageCondition` 기반 다형적 Condition 구조
- 여러 Condition의 AND 평가
- Tick 기반 Condition Polling
- Condition이 없는 Step 지원
- Action이 없는 Condition-only Step 지원

현재 구현된 Condition:

- `UStageCondition_Bool`
- `UStageCondition_ReachLocation`
- `UStageCondition_Interaction`
- `UStageCondition_DialogComplete`
- `UStageCondition_KillTarget`

### World Integration

- `SpawnActor` Action을 통한 World Actor 생성
- `SpawnLocationTargetTag`를 이용한 생성 위치 Actor 선택
- Player 위치 기반 `ReachLocation` Condition
- `IStageInteractable` 기반 Interaction 처리
- Actor별 `TargetTag`를 이용한 Stage Target 식별
- NPC Interaction 및 다중 Dialog 진행
- 상호작용 Actor의 중복 실행 방지 및 Blueprint 연출 이벤트 분리
- 보물상자 상호작용 콘텐츠 구성
- Player 이동 / 시점 / 기본 공격
- Simple Enemy AI
  - Idle
  - Chase
  - Attack
- `UStageHealthComponent` 기반 Player / Enemy HP 관리
- Damage / Death 처리
- Enemy `ContentId`별 Death Count Runtime 상태 관리
- Death Count 기반 `KillTarget` Condition 평가
- Enemy Death Animation 및 LifeSpan 기반 제거
- Reward Action을 통한 Player Gold 지급

### Combat & Animation

- Player / Enemy Attack Montage 적용
- Animation Notify 기반 실제 타격 시점 처리
- 공격 시점의 Forward LineTrace 기반 Hit 판정
- Player / Enemy별 유효 공격 대상 판정
- Player / Enemy 공격 판정과 Damage 처리 분리
- Enemy AI Target과 실제 Hit Target 역할 분리
- Enemy Death Montage 적용
- Enemy `AttackMontages` 배열에서 공격 Montage 랜덤 선택
- 공격 Montage 재생 중 중복 재생 방지
- 일반 Enemy와 보스에 공통 전투 로직 재사용
- 캡슐 반경과 AttackReach를 고려한 AI 공격 진입 거리 계산

### Camera & Sequence

- `UCameraShakeBase` 지정 및 PlayerCameraManager를 통한 Camera Shake 재생
- Delay Action으로 연출 사이의 진행 간격 구성
- `ULevelSequencePlayer`를 생성하여 Level Sequence 재생
- `OnFinished` 이벤트를 Action 완료 흐름에 연결
- Stage 실행기에 특정 Sequence의 재생 코드를 추가하지 않고 Action 데이터로 연동

### Objective Presentation

- Step별 Objective 데이터 관리
- Objective 변경 이벤트 기반 UI 갱신
- World Objective Marker 표시 / 숨김
- Objective 데이터 기반 Marker 위치 갱신
- Stage 시작 시점 차이를 고려한 초기 Objective 동기화
- Presentation 전용 Objective Marker의 Collision / Navigation 영향 제거

### Stage UI

- `UStageHUDWidget` 기반 Stage Gameplay UI 구성
- Objective / Player Health / Gold / Stage State UI
- Player / Enemy Overhead Health UI
- Stage State에 따른 Gameplay UI 표시 / 숨김
- Ready 상태에서 Stage ID 표시
- Playing 전환 시 Stage Start 표시
- Reward 표시 이후 Stage Clear 결과 표시
- Gold 변경 이벤트 기반 현재 Gold 갱신
- Reward 획득 시 Gold 증가 Animation 표시
- Health 변경 이벤트 기반 HP UI 갱신
- 다음 Stage 이동 버튼을 통한 전환 요청
- Level 전환 후 전달된 Gold를 현재값으로 복원하고 UI 갱신

### Stage Flow & Level Transition

- Stage 1 / Stage 2를 별도 Level로 구성
- StageCollection에서 Stage ID에 대응하는 DataAsset과 Level 조회
- 현재 Stage의 `NextStageId`를 이용하여 다음 Stage 선택
- `UStageFlowSubsystem`에서 Level 간 Stage 흐름 관리
- World 전환 시 현재 Stage Runtime을 새 World에서 생성
- 전환 전 Player Gold를 임시 보관하고 새 Level의 Player에 전달
- `ConsumePendingGold()`를 통한 일회성 Gold 전달

Gold 전달은 실행 중 Level 전환을 위한 메모리 상태 전달입니다. 디스크 기반 Save / Load는 현재 구현 범위에 포함하지 않습니다.

### Playable Stages

#### Stage 1 — VillageDefense

NPC Interaction → Dialog → 목표 지점 이동 → Enemy Spawn → Combat → KillTarget 3 → Reward → Stage Completed

NPC 대화, 이동 목표, 다수 Enemy 전투와 보상을 통합한 Stage입니다.

#### Stage 2 — Boss Encounter

별도 보스 Level에서 다음 요소를 조합한 Stage를 구성했습니다.

- Camera Shake / Delay / Level Sequence 연출
- 보스 생성 및 전투
- 보물상자 상호작용
- Reward 및 Stage Clear

두 Stage는 공통 Step / Action / Condition 구조를 사용합니다. Stage별 진행을 위한 분기를 `UStageInstance`에 추가하지 않고, Stage 데이터와 Level 배치·콘텐츠 에셋을 조합합니다.

### Debug

- 현재 Step 강제 완료 기능
- Exec Console Command를 통한 Stage 진행 테스트

## Runtime Flow

### Stage 실행

Stage Data 조회 → StageInstance 생성 → Prepare → Ready → Start → Playing → Start Step → Actions 순차 실행 → Conditions 평가 → Complete Step → 다음 Step 또는 Stage Completed

Action은 현재 Action의 완료 이벤트를 받은 뒤 다음 Action을 실행합니다. Delay는 Timer 만료 후, PlaySequence는 Sequence의 자연 종료 후 완료를 알립니다.

모든 Action 실행이 끝나면 Step의 Condition을 평가합니다. 모든 Condition이 만족되면 다음 Step으로 진행합니다.

Action 실행 실패 또는 Condition 평가가 Invalid인 경우 Stage는 Failed 상태로 전환됩니다. Completed 상태에서는 Gameplay UI를 정리하고 Stage 결과를 표시합니다.

### 다음 Stage 전환

다음 Stage 이동 요청 → NextStageId로 Collection 조회 → Player Gold 임시 보관 → 다음 Level 열기 → 새 World의 Stage Runtime 생성 → 새 Player에 Gold 전달 → 다음 Stage 진행

GameMode는 새 World에서 Flow Subsystem과 World Subsystem을 연결하는 초기화 역할을 담당합니다. 개별 Stage의 Step 진행은 StageInstance가 처리합니다.

## Architecture

| 구성 요소 | 책임 및 수명 |
| --- | --- |
| `UStageDataAsset` | Stage ID, NextStageId, Ready Duration, Steps와 Objective / Action / Condition 설정 정의 |
| `UStageCollectionDataAsset` | Stage ID에 대응하는 Stage DataAsset과 Level 조회 |
| `UStageFlowSubsystem` | GameInstance 수명에서 Stage 선택, Level 전환 조정, Gold 임시 전달 |
| `UStageSubsystem` | 현재 World의 StageInstance 생성·보유, Update 전달, Runtime 해제 |
| `UStageInstance` | 하나의 Stage Lifecycle, Current Step, Action 순서, Condition 판정, Objective 및 진행 상태 관리 |
| `FStageContext` | World Context, ContentId별 Enemy Death Count 등 실행 중 공유 상태 |
| GameMode | World 초기화 시 Flow와 World Subsystem 연결 |

### 책임과 의존성

- StageInstance는 개별 Stage의 진행을 담당하며 Flow Subsystem을 직접 참조하지 않습니다.
- World 단위 실행 관리와 Level 간 전환을 분리하여 서로 다른 수명에 배치합니다.
- Action / Condition은 공통 기반 타입을 통해 실행·평가하며, 구체 기능은 파생 클래스에 둡니다.
- Interaction은 NPC·보물상자의 구체 클래스 대신 `IStageInteractable`을 통해 처리합니다.
- UI는 Objective / Health / Gold 등의 변경 이벤트를 받아 표시를 갱신합니다.

### Character & Gameplay

- Player Character: 이동 / 공격, Gold 상태 관리 및 전환 후 Gold 복원
- Enemy Character: 공격 / Damage / Death, ContentId 기반 Death Event 전달
- Enemy AI Controller: Target 탐색, Idle / Chase / Attack 상태 관리, 이동 및 공격 요청
- Health Component: HP 상태, Damage 처리, Health 변경 이벤트
- Character Health Widget: 전달받은 Health Component 상태 표시

## Design Decisions

### 데이터와 실행 흐름의 분리

Stage별 Objective / Action / Condition 조합은 DataAsset에 두고, 공통 실행 순서는 StageInstance에서 관리합니다. 기존 기능으로 새 Stage를 구성할 때 실행기의 Stage별 분기를 늘리지 않는 것이 목적입니다.

Stage 인덱스와 처치 수 등의 진행 상태는 Runtime에 두고 있습니다. 다만 현재 Action / Condition 객체 내부의 Timer·SequencePlayer·Actor 캐시까지 실행별로 분리한 상태는 아니며, 6주차 보완 대상입니다.

### 다형성을 통한 기능 확장

StageInstance는 구체 Action의 연출·보상·생성 구현 대신 기반 타입의 실행 및 완료 계약을 사용합니다. CameraShake / Delay / PlaySequence 역시 이 흐름에 연결했습니다.

이는 기존 실행 흐름을 유지하면서 기능을 추가하는 확장 방식입니다. 대신 각 Action이 완료·실패를 일관되게 알리고 실행 자원을 정리해야 하므로, 비동기 Action의 수명 관리가 필요합니다.

### 상호작용과 콘텐츠 연출의 분리

Condition은 `IStageInteractable`을 통해 상호작용 상태를 확인합니다. 개별 Actor의 Blueprint 연출은 Actor 측에 두어 Stage 진행 판정과 콘텐츠 표현을 분리합니다.

### 수명에 따른 Subsystem 분리

현재 World의 Stage 실행은 World Subsystem에서 관리하고, World가 바뀌어도 필요한 Stage 선택 정보와 Gold 전달은 GameInstance Subsystem에서 관리합니다.

Gold는 Player가 보유하고 Flow가 전환 시점에 전달하는 최소 구조를 선택했습니다. 현재 단일 플레이어 흐름에는 간단하지만, Player와 Flow 사이에 직접 의존성이 있습니다.

## Verification

5주차 개발 중 다음 정상 진행 흐름을 확인했습니다.

- Stage 1에서 Stage 2로 Level 전환
- 전환 전후 Player Gold 유지
- Stage 2의 연출·보스 전투·보물상자·보상·Stage Clear 통합 흐름
- 일반 Enemy와 보스의 공격 Montage 재생
- 보스 공격 Montage 3종 및 재생 중 중복 방지

실패·중단·강제 Step 이동 등 비동기 경계 상황과 패키징 실행은 최종 검증 대상으로 남아 있습니다.

## Current Limitations

- 비동기 Action 대기 중 Stage 실패 또는 Step 강제 완료 시, 이전 완료 이벤트가 진행에 영향을 주지 않도록 검증·취소·정리 보완 필요
- DataAsset에 포함된 Action / Condition의 실행별 Runtime 객체 분리 필요
- Sequence 자연 종료 외 외부 Stop / 취소 경로 보완 필요
- Level 전환 실패, 중복 요청 및 Gold 캡처 실패 처리 보완 필요
- Stage 2 Level 직접 실행 시 Stage 데이터 선택 정책 정리 필요
- Save / Load는 보류

## Project Structure

전체 프로젝트의 Stage 관련 원본 경로는 `StageCraft/Source/StageCraft/Stage/`입니다. 현재 공개 저장소에는 이 경로 아래 대표 파일만 포함합니다. 향후 필요에 따라 다른 프로젝트 파일을 같은 원본 구조로 추가할 수 있습니다.

`StageCraft/Source/StageCraft/Stage/`

- `Data/`: Stage / Collection / Step / Objective 정의
- `Runtime/`: Stage 실행, World 관리, Level 간 Stage 흐름 및 Runtime Context
- `Action/`: 로그 / 생성 / 대화 / 보상 / Camera Shake / Delay / Sequence
- `Condition/`: Stage 완료 조건 정의 및 평가
- `Actor/`: Player / NPC / Enemy / Interaction / Health / World Marker
- `Animation/`: Attack Hit Animation Notify
- `UI/`: Stage HUD / Objective / Health / Gold / Stage State UI

## Roadmap

### Week 6 — Runtime Reliability

- 비동기 Action 완료 검증 및 종료·취소 정리
- Action / Condition 실행별 Runtime 상태 분리
- Level 전환 오류 처리 및 초기 Stage 선택 정책 정리
- 실패·중단·강제 Step 이동을 포함한 통합 테스트

### Developer Support

- Debug Tool 확장
- Stage / Collection 데이터 검증
- 시간 값, Target 설정, Stage 연결 등 잘못된 데이터 확인

### Portfolio Polish

- Final Integration Testing 및 패키징 실행 확인
- Architecture / Runtime Flow 문서화
- 대표 코드와 설계 선택·Trade-off 정리
- 두 Stage 시연 영상 및 스크린샷
- Portfolio PDF

### Deferred

- 최소 범위의 Save / Load
