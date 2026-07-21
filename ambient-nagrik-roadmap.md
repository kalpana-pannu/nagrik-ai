# Ambient NAGRIK — Roadmap

This document captures the planned evolution of NAGRIK AI from a reactive complaint platform into a proactive, edge-intelligent civic monitoring system. Nothing in this document is implemented yet — this is a roadmap, not a build log. Working code lives in `hardware/sense-box/`.

## NAGRIK AI (Current — Original Version)

**What it is:** India's first AI-native Civic Intelligence Platform, combining IoT hardware (NAGRIK SENSE BOX) with cloud AI to help citizens interact with government and report civic issues.

**Core philosophy:** "The smartest government officer that never sleeps."

**How it works:**
```
Citizen → Files Complaint → System Receives → Routes to Department → Gets Resolved
         (WhatsApp/App)    (Cloud AI)        (Manual/System)
```

**Key features:**
| Feature | Description |
|---|---|
| Citizen side | WhatsApp complaint filing, 15 Indian languages, photo upload, real-time tracking |
| Government side | Live city heatmap, complaint auto-routing, SLA tracker, daily reports |
| Hardware | NAGRIK SENSE BOX with sensors (DHT22, MQ-135, LDR, SW-420, KY-038, rain, buzzer) |
| Intelligence | Cloud-based AI (Claude API, OpenAI Whisper) — processes complaints and routes them |
| Trigger | Human-initiated — citizen must report the problem |

**Tech stack:**
| Layer | Technology |
|---|---|
| Hardware | ESP32, sensors, solar power, IP65 enclosure |
| AI | Claude API, OpenAI Whisper (cloud-based) |
| Backend | Node.js, FastAPI (Python) |
| Database | PostgreSQL, InfluxDB |
| Frontend | React.js, React Native, Tailwind CSS |
| IoT | Blynk, MQTT, AWS IoT Core |

**Strengths:**
- Bharat-first design — WhatsApp, 15 languages, solar power, low cost
- Full-stack ownership — hardware + software, not just an app
- Market-ready — directly solves a known civic complaint problem
- Government-friendly — built for Indian municipal workflows

**Weaknesses:**
- Reactive — waits for citizens to complain; problems can exist for days/weeks before being reported
- Cloud-dependent — needs internet for AI processing, which is a barrier in rural areas
- Higher latency — internet round-trip delays decisions
- Ongoing API costs that scale with usage
- No local intelligence — ESP32 currently just transmits data, doesn't "think"

## Ambient NAGRIK (Planned — Upgraded Version)

**What it is:** The evolution of NAGRIK AI into a self-aware city nervous system that continuously perceives, predicts, and acts without human intervention, using TinyML on the edge.

**Core philosophy:** "Cities that take care of themselves — sensing, predicting, and acting without human intervention."

**How it would work:**
```
Sensors → Continuous Monitoring → Local AI (TinyML) → Anomaly Detected → Autonomous Action
(ESP32)   (24/7)                  (On-device)         (Predicts failure)  (Work order/Alert)
```

**Planned features:**
| Feature | Description |
|---|---|
| Edge AI | TensorFlow Lite Micro running on ESP32 — decisions in milliseconds |
| Zero-cloud dependency | Works without internet — suited for rural deployment |
| Predictive intelligence | Learns "normal" patterns and predicts failures before they happen |
| Self-healing | Automatically triggers maintenance without human input |
| Low power | Optimized for solar + battery — long deployment life |
| Privacy by design | Raw data never leaves the device — only insights do |

**Stack comparison:**
| Layer | NAGRIK AI (current) | Ambient NAGRIK (planned) |
|---|---|---|
| AI location | Cloud (Claude API) | Edge (TinyML on ESP32) |
| AI model | Large Language Model | TensorFlow Lite Micro (quantized) |
| Decision speed | Seconds to minutes | Milliseconds |
| Internet dependency | High (required) | Low (optional) |
| Cost model | Pay-per-use API | One-time deployment |
| Privacy | Data leaves device | Data stays on device |

**Code shift example:**

Current (rule-based):
```cpp
if (vibration > THRESHOLD) {
    sendAlert();
}
```

Planned (TinyML-based):
```cpp
float anomaly_score = model->Predict(sensor_data);
if (anomaly_score > 0.85) {
    trigger_action();  // Autonomous decision
    sendToCloud();      // Log for analytics
}
```

**Architecture shift:**
```
NAGRIK AI (Cloud-Centric):
Sensors → ESP32 → Internet → Cloud AI → Action → User

Ambient NAGRIK (Edge-Centric):
Sensors → ESP32 (TinyML) → Action (Local)
                          ↘ Cloud (Analytics only)
```

## Why Ambient NAGRIK Is the Long-Term Direction

1. **Solves a deeper problem** — prevention, not just faster complaint handling
2. **Works where it's needed most** — rural India often lacks reliable internet; Ambient NAGRIK would work offline
3. **Cheaper to scale** — no recurring per-request API costs after deployment
4. **Privacy-first** — data stays on-device, which matters for government adoption
5. **Harder to copy** — TinyML + hardware expertise is a real, defensible moat
6. **Aligned with global trends** — ambient intelligence is a fast-growing market category

## Path Forward

| Phase | Focus |
|---|---|
| Now | Finish the combined sketch with all working sensors on NAGRIK AI |
| Next | Learn and install TinyML (TensorFlow Lite Micro) on ESP32 |
| Then | Collect baseline sensor data for model training |
| Finally | Deploy a trained model and demonstrate autonomous anomaly detection |

NAGRIK AI is the working product. Ambient NAGRIK is where it's headed.
