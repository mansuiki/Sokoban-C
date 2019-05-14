# Sokoban Game With C

|  조원   |    학번    |                              Github                               |
| :---: | :------: | :---------------------------------------------------------------: |
|  김경택  | 20192805 |              [mansuiki](https://github.com/mansuiki)              |
|  김현우  | 20192822 | [qualificationalitated](https://github.com/qualificationalitated) |  |
|  이정현  | 20192863 |                 [5d-jh](https://github.com/5d-jh)                 |  |
|  박민수  | 20192829 |          [parkminsoo99](https://github.com/parkminsoo99)          |  |

------------
## What We Need to Do?
1. [x] 맵 읽기 / 맵크기에 맞는 배열 생성
2. [ ] 순위표
3. [ ] 되돌리기 기능
4. [ ] `캐릭터 움직이기`
5. [ ] `상자 밀기`
6. [ ] 현재 상태 저장 & 불러오기


|  조원   |    학번    |     역할      |
| :---: | :------: | :---------: |
|  김경택  | 20192805 | 캐릭터 움직이기 구현 |
|  김현우  | 20192822 |   되돌리기 구현   |
|  이정현  | 20192863 | Save 기능 구현  |
|  박민수  | 20192829 |   순위표 구현    |


------------
## 개발 정보
* 개발은 `branch` 생성 후 나중에 `merge` 해야함 -> 기본적으로 `origin/master` 사용 자제

> ### branch 사용법
> 1. branch 생성
> ```zsh
> git branch branch_name # branch_name 가지를 생성
> ```
> 2. branch 전환
> ```zsh
> git branch # branch 리스트 출력
> git checkout branch_name # branch_name 가지로 전환
> git push -u origin branch_name # 처음 push 한다면 이 명령을 수행해야함
> # 이제 사용!
> ```

* 개발 시작하기 전에 `pull` 하자!
> ### pull 사용법
> ```zsh
> git pull # pull 원하는 가지나 master 로 이동후 실행
> ```

* `commit` & `push`의 생활화
> ### commit 규칙
> ```zsh
> # 예시 : README.md 파일을 수정시
> README.md 조원 이름 오타 수정 # 짧은 요약 1줄
> 김경태 오타를 김경택  으로 수정함 # 필요시 세부내용 작성 (여러줄도 작성 할 수 있음)
> ```
> ### commit & push
> ```zsh
> git commit # commit 창이 뜸
> # commit 내용 입력
> # commit 입력창 닫힘
> git push # push! (push 하기전에 확인 필수!)
> ```
