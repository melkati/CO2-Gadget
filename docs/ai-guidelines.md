# AI-Assisted Contributions

> **TL;DR:** AI tools are welcome as productivity aids, but **you are responsible for every line of code you submit**. If you can't explain it, don't submit it.

---

## Philosophy

CO₂ Gadget is an embedded firmware project where hardware-specific behavior, power constraints,
and sensor timing matter. A bug caused by misunderstood AI-generated code can mean:

- A sensor that silently reports wrong CO₂ readings
- A device that won't wake from deep sleep
- An OTA update that bricks the board

This is not a web app where a bad PR can be hotfixed in minutes — embedded firmware has real
consequences. Our review bandwidth is limited, and low-quality contributions drain energy from
everyone.

**AI is a tool**, like a linter, a debugger, or a compiler. It amplifies your ability, but it
does not replace your judgment. Use it wisely, own the result.

---

## ✅ What AI Can Help With

These are good uses of AI tools (LLMs, Copilot, ChatGPT, etc.):

| Use Case | Example |
|---|---|
| **Understanding existing code** | "Explain what `RTC_DATA_ATTR` does in this function" |
| **Boilerplate generation** | "Generate a getter/setter pattern for a new NVS preference" |
| **Debugging assistance** | "Why might this I²C read return -1 on deep-sleep wake?" |
| **Documentation** | Drafting docstrings, comments, or README updates |
| **Refactoring with supervision** | "Extract this sensor init block into a helper function" — review the result |
| **Finding patterns** | "Show me how other sensors handle the warm-up callback" |
| **Drafting tests** | "Generate a test sketch for the threshold manager" — verify it compiles and makes sense |

In all cases: **read, understand, and test the output** before committing.

---

## ❌ What NOT to Do

These are **never acceptable**:

| ❌ Don't | Why |
|---|---|
| **Submit unverified AI code** | A PR where you didn't read or understand the code will be closed without review. We can tell. |
| **Generate security reports with AI** | Hallucinated vulnerabilities waste everyone's time and may result in a ban. |
| **Use AI to review other people's PRs** | AI code review lacks context about hardware, power, and sensor behavior. Human review only. |
| **Submit AI-generated "drive-by" PRs** | Mass-generated PRs from AI with no prior engagement with the project. |
| **Copy code from unknown sources** | Even if AI writes it, you must know where the patterns come from. Plagiarism is unacceptable. |
| **Use AI to bypass learning** | If you don't understand the change, you can't maintain it when something breaks. |

---

## Rules

1. **You are responsible for everything you submit.** Regardless of how the code was generated —
   by you, by an LLM, by a code generator, or by copy-paste — the maintainers will hold you
   accountable for its correctness, safety, and style.

2. **Disclosure is a courtesy, not a requirement.** If a significant portion of your
   contribution was AI-generated, consider mentioning it in your PR description. This helps
   reviewers understand what to look at. See [example below](#disclosure-example).

3. **Quality over speed.** A well-crafted human PR that compiles and works is infinitely more
   valuable than a dozen AI-generated PRs that don't.

4. **Test on real hardware when possible.** Embedded firmware has emulation limits. If your
   change affects sensors, displays, deep sleep, or communication protocols, test it on an
   actual board.

5. **Open an issue first for major changes.** This applies to all contributions, AI-assisted
   or not. Discuss before implementing.

---

## What Happens If You Don't Follow These Guidelines

- PRs that appear to be unverified AI output will be **closed without review**.
- Repeated low-quality submissions may result in a **temporary or permanent ban** from
  contributing.
- AI-generated security reports will be **closed without response** and may result in a ban.

We're not looking for a reason to reject contributions — we're looking for contributors who
care about the quality of their work.

---

## Disclosure Example

If you used AI tools and want to mention it, a brief note in the PR description is sufficient:

> **AI assistance:** I used GitHub Copilot to draft the I²C initialization block in
> `CO2_Gadget_Sensors.h`. I reviewed every line, tested it on a TTGO T-Display with SCD30,
> and verified it compiles cleanly for all board flavors.

This tells the reviewer: "I used a tool, but I did my due diligence."

---

## Why This Policy?

This project is maintained by a small team (mostly one person) with limited review bandwidth.
Every hour spent reviewing a low-quality AI PR is an hour not spent fixing real bugs,
supporting users, or adding features.

We believe AI *can* make open-source more productive — but only when used responsibly.
This policy exists to protect the project's quality and the maintainer's time, not to
discourage good-faith contributions.

---

## References

This policy was inspired by similar guidelines from:

- [OCaml AI Policy (AI.md)](https://github.com/ocaml/ocaml/blob/trunk/AI.md)
- [Django AI Assistance Disclosure](https://docs.djangoproject.com/en/dev/internals/contributing/writing-code/submitting-patches/#ai-assistance-disclosure)
- [ESPHome AGENTS.md](https://github.com/esphome/esphome/blob/dev/AGENTS.md)
