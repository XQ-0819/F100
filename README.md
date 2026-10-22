# 嵌入式项目模板

本仓库用于通过 Gitee 模板创建公司内部 STM32、RTOS 和嵌入式 C 项目，内置 Codex 原生规则入口、团队协作资料、代码审查规则、Git 文件策略及 Windows 工程清理脚本。通过本模板创建项目时，应完整保留根目录 `AGENTS.md`、`.agents/` 和 `.team/`，使各项目都具备一致的规则入口、仓库级 Skills 与协作流程。

## 模板内容

- `AGENTS.md`：Codex 原生识别的项目级智能编程与嵌入式 C 编码规则；文件名必须保持全大写。
- `.team/rules/code_review.md`：团队代码审查规则，由 `AGENTS.md` 要求在审查任务中主动读取。
- `.team/rules/.rules-version`：团队规则包版本。
- `.team/嵌入式软件功能开发Codex协作SOP与提示词模板_V1.8_20260818.md`：完整功能开发、纠偏、阶段交付、提交前最终审查和 PR 文案整理时使用的团队协作 SOP。
- `.team/Codex仓库级Skills使用指南_V1.2_20260818.md`：说明三个仓库级 Skills 的职责、调用时机、衔接顺序和完整使用示例。
- `.agents/skills/embedded-feature-plan/`：Codex 原生仓库级功能方案 Skill；使用 `$embedded-feature-plan` 可在编码前调查调用链、现有 API 和影响范围并输出最小修改方案。
- `.agents/skills/embedded-release-delivery/`：Codex 原生仓库级交付 Skill；使用 `$embedded-release-delivery` 可依据实际代码、Git 差异和验证记录整理五份交付文档或 PR 文案。
- `.agents/skills/embedded-final-review/`：Codex 原生仓库级最终审查 Skill；使用 `$embedded-final-review` 可对明确候选差异执行独立只读审查并给出提交门禁结论。
- `.gitattributes`：字符编码、行尾和二进制文件属性。
- `.gitignore`：编译产物、IDE 私有文件及临时文件策略。
- `autoclear.bat`：Windows 下的工程生成文件清理工具。

## 适用范围

本模板默认适用于 STM32、FreeRTOS、uC/OS、裸机及相关嵌入式 C 项目。非 C 项目、非 STM32 平台、第三方代码和自动生成代码应根据实际约束调整规则，不应机械套用。

## 创建项目后的检查

1. 确认目标芯片、操作系统、编译器和工程目录符合项目实际情况。
2. 确认 `AGENTS.md` 中的命名、排版、栈阈值和安全要求适用于本项目。
3. 确认 `.team/rules/` 和 `.team/` 下的协作 SOP 已随模板进入项目；`.team/` 不是 Codex 原生规则目录，其内容通过根目录 `AGENTS.md` 按任务加载。
4. 确认 `.agents/skills/embedded-feature-plan/`、`.agents/skills/embedded-release-delivery/` 和 `.agents/skills/embedded-final-review/` 已随模板进入项目，并可通过对应 `$skill-name` 显式调用。
5. 确认 BIN/HEX 提交策略符合项目发布和保密要求。
6. 确认源文件原有编码、BOM 和行尾未因工具设置发生无关变化。
7. 在首次使用 `autoclear.bat` 前确认其清理范围。

## 文件编码与行尾

- 修改已有 C、头文件、汇编、链接脚本和工程文件时，必须保持原有字符编码、BOM 状态和行尾。
- 原文件为 ANSI 或项目代码页时，不得仅因编辑器默认设置转换为 UTF-8。
- 新建上述工程文件在没有项目特殊要求时采用公司默认 ANSI 代码页；协议或工具明确要求 UTF-8 时按其规范执行。
- Markdown、YAML、JSON 等模板配置统一使用 LF；BAT/CMD 使用 CRLF。
- CSV 按原始字节保存，以保留 UTF-8 BOM 和混合行尾等工具格式要求。

## 固件与敏感信息

模板允许跟踪 `*.bin` 和 `*.hex`，用于保存经确认的可烧录固件。提交前必须确认固件不包含私钥、设备唯一凭据、生产密钥、默认口令或未经批准的客户数据，并确保固件可追溯到对应源码、构建配置和工具链版本。

不得提交 `.env`、私钥、证书私密部分、签名密钥、内存转储或其他敏感文件。

## 清理脚本

`autoclear.bat` 仅支持 Windows，在脚本所在项目目录中递归清理明确列出的编译生成文件和临时文件，并复用 `simulator/clean.ps1 -Target Build` 清理模拟器构建目录和错误日志。

Doxygen 处理规则：

1. 检查 `html/readme.chm`；
2. 先复制为根目录临时文件；
3. 成功后替换根目录 `readme.chm`；
4. 最后删除 `html` 目录。

`readme.chm` 和 `html/` 都属于构建产物，不提交 Gitee。脚本不会删除通用 `*.bak`、`*.orig`、`*.sct` 或普通 HTML 文件，但会递归删除其清单中匹配的生成文件，运行前应确认项目中没有同名业务文件。

## 规则版本

规则包采用语义化版本：

- 主版本：不兼容的规则、命名或审查策略变更；
- 次版本：向后兼容地增加规则或检查项；
- 修订版本：文字修正、格式修复和不改变含义的调整。

修改 `AGENTS.md`、`.team/rules/code_review.md`、团队协作 SOP、仓库级通用 Skill 或会影响规则执行的模板策略时，应同步更新 `.team/rules/.rules-version`。派生项目可对比该版本与公司模板仓库版本，并按项目实际情况人工合并更新。当前规则版本为 `1.2.0`。
