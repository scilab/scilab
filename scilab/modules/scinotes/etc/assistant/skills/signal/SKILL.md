---
name: signal
description: |
  Signal processing in Scilab: FFT and spectra, window functions, FIR/IIR
  filter design and application, convolution, resampling. Scilab function
  names differ from MATLAB's (no butter/filtfilt/freqz/fir1). Trigger words:
  "fft", "fourier", "spectrum", "frequency", "filter", "low-pass", "window",
  "convolution", "sampling".
triggers: fft, ifft, fourier, spectre, spectrum, fréquence, frequency, filtre, filter, filtrage, passe-bas, passe-haut, low-pass, high-pass, band-pass, convolution, fenêtrage, hanning, hamming, kaiser, signal, échantillonnage, sampling, harmonique, harmoniques, périodogramme, periodogram, bruité, bruit, denoise, débruiter, fréquences, spectrogramme, spectrogram, enveloppe, envelope, hilbert, pics, peaks, findpeaks, butter, filtfilt, freqz, fir1, pwelch, rééchantillonner, resample, décimation, decimate, moyenne glissante, moving average, rms, chirp, dsp, psd, densité spectrale, iir, fir, butterworth, chebyshev, déphasage, phase nulle, zero-phase
---

# Scilab signal processing

Scilab is NOT MATLAB here: `butter`, `filtfilt`, `freqz`, `fir1`, `fir2`,
`pwelch`, `periodogram`, `spectrogram`, `resample`, `decimate`,
`findpeaks`, `medfilt1`, `movmean`, `chirp`, `square`, `sawtooth`, `rms`,
`envelope`, `lowpass`/`bandpass` do NOT exist. Use the mappings below.

| Task | MATLAB (DO NOT USE) | Scilab |
|------|---------------------|--------|
| FFT | `fft` | `fft` (same) |
| Window | `hanning(n)` | `window('hn', n)` |
| FIR design | `fir1` | `wfir` |
| IIR design | `butter`, `cheby1`, `ellip` | `iir` (ftype + design family) |
| Apply filter | `filter(b,a,x)` | `filter(b,a,x)` or `flts(x, hz)` |
| Zero-phase filter | `filtfilt` | none — apply `flts` forward, reverse, apply again, reverse |
| Frequency response | `freqz` | `[mag, fr] = frmag(hz, npts)`, or `[frq, rep] = repfreq(hz, fmin, fmax)` (frequencies FIRST), or `horner(hz, exp(%i*w))` |
| Spectral estimate | `pwelch` | `pspect` |
| Cross spectrum | `cpsd` | `cspect` |
| Analytic signal / envelope | `hilbert`, `envelope` | `hilbert(x)` (exists), envelope = `abs(hilbert(x))` |
| Resample | `resample`, `decimate` | `intdec(x, ratio)` |
| Peaks | `findpeaks` | by hand: `k = find(x(2:$-1) > x(1:$-2) & x(2:$-1) >= x(3:$)) + 1` + threshold |
| Moving average | `movmean` | `filter(ones(1,m)/m, 1, x)` or `convol` |
| RMS | `rms` | `sqrt(mean(x.^2))` |
| Chirp / square | `chirp`, `square` | build from `sin`: `sin(2*%pi*(f0*t + (f1-f0)/(2*T)*t.^2))`, `sign(sin(...))` |
| Spectrogram | `spectrogram` | loop of windowed `fft` on frames, then `Matplot`/`grayplot` |

## FFT and spectrum with a real frequency axis

```scilab
fs = 1000;                       // sampling frequency [Hz]
t  = 0:1/fs:1-1/fs;              // 1 s of signal
x  = sin(2*%pi*50*t) + 0.5*sin(2*%pi*120*t) + 0.2*rand(t, "normal");

n = length(x);
X = fft(x);
f = (0:n/2-1) * fs / n;          // one-sided frequency axis
A = abs(X(1:n/2)) * 2 / n;       // one-sided amplitude spectrum
plot(f, A);
xlabel("Frequency (Hz)"); ylabel("Amplitude");
```

- `fft(x)` along a matrix: `fft(A, -1, dim)`; inverse: `ifft(X)` (or `fft(X, 1)`).
- `fftshift(X)` centers the zero frequency for two-sided plots.
- Remove trends before analysis with `detrend(x)` (constant or linear).

## Windows

`win = window(type, n)` with types `'re'` (rect), `'tr'` (triangular),
`'hn'` (Hanning), `'hm'` (Hamming), `'kr'` (Kaiser, needs parameter),
`'ch'` (Chebyshev). Apply by element-wise product: `xw = x .* window('hn', length(x))`.

## FIR filters — wfir

```scilab
// [wft, wfm, fr] = wfir(ftype, forder, cfreq, wtype, fpar)
// ftype: "lp" | "hp" | "bp" | "sb";  cfreq: NORMALIZED cutoffs in [0, 0.5)
[h, hm, fr] = wfir("lp", 33, [0.2 0], "hm", [0 0]);  // low-pass, order 33, Hamming
y = convol(h, x);                 // or filter(h, 1, x)
plot(fr, hm);                     // designed magnitude response
```

Cutoff normalization: `cfreq = f_hz / fs` (NOT `f_hz / (fs/2)` as in MATLAB).
The `wfir` cutoffs are the ~-6 dB points (middle of the transition band),
not the passband edges: for a flat passband up to f, design at f + half
the transition width (Hamming: transition ≈ 3.3·fs/N Hz).

## IIR filters — iir

```scilab
// hz = iir(n, ftype, fdesign, frq, delta)
// ftype: "lp"|"hp"|"bp"|"sb"; fdesign: "butt"|"cheb1"|"cheb2"|"ellip"
// frq: normalized in [0, 0.5); delta: ripple parameters (can be [0 0] for butt)
hz = iir(4, "lp", "butt", [0.1 0], [0 0]);   // 4th-order Butterworth low-pass
y  = flts(x, hz);                            // filter the signal
[hzm, fr] = frmag(hz, 256);                  // magnitude response
plot(fr, hzm);
```

`hz` is a discrete transfer function (rational in z). To use `filter`,
the coefficients must be in z^-1 — `coeff` gives ASCENDING powers of z, so
`filter(coeff(hz.num), coeff(hz.den), x)` is WRONG (it diverges). Convert:
```scilab
a = coeff(hz.den);  b = coeff(hz.num);
b = [zeros(1, length(a) - length(b)), b];     // pad numerator to den degree
b = b($:-1:1) / a($);  a = a($:-1:1) / a($);  // descending powers, a(1) = 1
y = filter(b, a, x);                          // == flts(x, hz)
```
Simpler: keep `flts(x, hz)`. Analog prototypes: `analpf`; direct
pole/zero form: `[p, z, g] = iir(...)`.

Zero-phase (filtfilt equivalent):
```scilab
yf = flts(x, hz);  yb = flts(yf($:-1:1), hz);  y0 = yb($:-1:1);
```
(magnitude response squared; transients at both ends).

## Convolution and correlation

- `convol(h, x)` — full convolution (also `conv` for MATLAB-style).
- `xcorr(x, y)` — cross/auto-correlation; `corr` for correlation coefficients.

## Resampling

- `intdec(x, lom)` — rational rate change (interpolation/decimation).
- Simple decimation: `x(1:m:$)` AFTER low-pass filtering below the new Nyquist.

## Spectral estimation

```scilab
sm = pspect(100, 200, "hn", x);   // sectioning 200-pt, overlap 100, Hanning
f  = (0:99) / 200 * fs;           // matching axis for the first half
plot(f, sm(1:100));
```

## Pitfalls

- All design frequencies are normalized to the SAMPLING frequency
  (range [0, 0.5)), not to Nyquist.
- `hilb` is the Hilbert MATRIX; the Hilbert transform / analytic signal is
  `hilbert(x)`.
- Add `clf()` before re-plotting spectra in loops, and always check the
  result with get_figure.
- Report frequencies and amplitudes from COMPUTED values (peak detection
  on the spectrum), never by reading them off the plot image: estimates read
  from a figure are often wrong (e.g. "~20 and ~65 Hz" for 25/50/75 Hz).
