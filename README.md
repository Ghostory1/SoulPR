# UE5 소울라이크 게임 모작 프로젝트

![Unreal Engine 5.5](https://img.shields.io/badge/Engine-Unreal%205.5-blue)
![Language](https://img.shields.io/badge/Language-C%2B%2B-blue)

## 🧠 개요

이 프로젝트는 Unreal Engine 5.5와 C++를 기반으로 한 **소울류(Soulslike) 게임 모작**입니다.  
플레이어와 적, 보스 간의 정교한 전투 시스템을 구현하는 데 초점을 두었습니다.

### 🔗 프로젝트 문서 (Notion)
[📘 정리된 문서 바로가기](https://www.notion.so/208f2a3a5fb580648632e2cf3efcadb8?v=208f2a3a5fb58155800f000c7e2b87b5&source=copy_link)

## 🎮 주요 기능

- 🧍‍♂️ 플레이어 캐릭터
- ⚔️ 무기 장착 및 교체 시스템
- 🧟 일반 적 AI (기본 공격/추적)
- 👹 보스 AI (패턴 기반 전투)
- 🌀 회피 (구르기) 시스템
- 🛡️ 방어 및 패링 (타이밍 기반 반격)
- 🍯 포션(회복 아이템)
- 🕹️ 애니메이션 시스템 (노티파이 및 블렌드)
- 🎮 전반적인 전투 조작 감각 구현

## 🗂️ 프로젝트 구조 예시

```bash
SoullikeProject/
├── Content/
│   ├── Characters/
│   │   ├── Player/
│   │   ├── Enemies/
│   │   └── Boss/
│   ├── Blueprints/
│   ├── Animations/
│   └── Weapons/
├── Source/
│   └── SoullikeProject/
└── SoullikeProject.uproject
