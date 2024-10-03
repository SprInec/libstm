import numpy as np
import matplotlib.pyplot as plt

def generate_triangle_wave(frequency, amplitude, phase, duration, sampling_rate, duty_cycle=0.5):
    t = np.linspace(0, duration, int(duration * sampling_rate), endpoint=False)
    triangle_wave = amplitude * (2 * np.abs(2 * (t * frequency + phase - np.floor(t * frequency + phase + 0.5))) - 1)
    return t, triangle_wave

def generate_sine_wave(frequency, amplitude, phase, duration, sampling_rate):
    t = np.linspace(0, duration, int(duration * sampling_rate), endpoint=False)
    sine_wave = amplitude * np.sin(2 * np.pi * frequency * t + phase)
    return t, sine_wave

def plot_waveform(ax, t, waveform, title, xlabel, ylabel):
    ax.plot(t, waveform)
    ax.set_title(title)
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.grid(True)

def generate_and_plot_signals(frequency_tri, amplitude_tri, phase_tri, duration_tri, sampling_rate_tri,
                              frequency_sine, amplitude_sine, phase_sine, duration_sine, sampling_rate_sine):
    # Generate triangle wave
    t_tri, triangle_wave = generate_triangle_wave(frequency_tri, amplitude_tri, phase_tri, duration_tri, sampling_rate_tri)

    # Generate sine wave
    t_sine, sine_wave = generate_sine_wave(frequency_sine, amplitude_sine, phase_sine, duration_sine, sampling_rate_sine)

    # Add sine wave and triangle wave to get the new waveform
    new_waveform = sine_wave + triangle_wave

    # Compute the FFT for the new waveform
    fft_result_new = np.fft.fft(new_waveform)
    fft_freqs_new = np.fft.fftfreq(len(t_sine), d=1/sampling_rate_sine)
    fft_result_new = np.fft.fftshift(fft_result_new)
    fft_magnitude_new = np.abs(fft_result_new)

    # Create a 2x2 grid for subplot
    fig, axs = plt.subplots(3, 1, figsize=(12, 12), sharex=True)

    # Plot the triangle wave
    plot_waveform(axs[0], t_tri, triangle_wave, "Triangle Wave", "Time (s)", "Amplitude (V)")

    # Plot the sine wave
    plot_waveform(axs[1], t_sine, sine_wave, "Sine Wave", "Time (s)", "Amplitude (V)")

    # Plot the new waveform
    plot_waveform(axs[2], t_sine, new_waveform, "New Waveform (Sine + Triangle)", "Time (s)", "Amplitude (V)")

    # Plot the frequency spectrum for the new waveform
    fig, ax = plt.subplots(figsize=(8, 5))
    ax.plot(fft_freqs_new, fft_magnitude_new)
    ax.set_title("Frequency Spectrum (New Waveform)")
    ax.set_xlabel("Frequency (Hz)")
    ax.set_ylabel("Magnitude")
    ax.grid(True)

    # Adjust layout and display the plots
    plt.tight_layout()

    def on_xlims_change(axes):
        for ax in axs:
            ax.set_xlim(axs[2].get_xlim())

    # Connect the function to the event handler
    fig.canvas.mpl_connect('xlim_changed', on_xlims_change)

    plt.show()


# 三角波-1
frequency_tri = 50    # Frequency of the triangle wave (100 kHz)
amplitude_tri = 1      # Amplitude of the triangle wave (1 Vpp)
phase_tri = 0          # 0 phase (in seconds)
duration_tri = 0.1     # Duration of the wave (100 ms)
sampling_rate_tri = 1600  # 1.6 MHz sampling rate (number of samples per second)

# 三角波-2
frequency_tri2 = 50    # Frequency of the triangle wave (100 kHz)
amplitude_tri2 = 1      # Amplitude of the triangle wave (1 Vpp)
phase_tri2 = 0          # 0 phase (in seconds)
duration_tri2 = 0.1     # Duration of the wave (100 ms)
sampling_rate_tri2 = 1600  # 1.6 MHz sampling rate (number of samples per second)

# 正弦波-1
frequency_sine = 100    # Frequency of the sine wave (50 kHz)
amplitude_sine = 1      # Amplitude of the sine wave (0.5 Vpp)
phase_sine = 0            # 0 phase (in seconds)
duration_sine = 0.1       # Duration of the wave (100 ms)
sampling_rate_sine = 1600  # 1.6 MHz sampling rate (number of samples per second)

# 正弦波-2
frequency_sine2 = 100    # Frequency of the sine wave (50 kHz)
amplitude_sine2 = 1      # Amplitude of the sine wave (0.5 Vpp)
phase_sine2 = 0            # 0 phase (in seconds)
duration_sine2 = 0.1       # Duration of the wave (100 ms)
sampling_rate_sine2 = 1600  # 1.6 MHz sampling rate (number of samples per second)

# 三角+正弦
# generate_and_plot_signals(
#     frequency_tri, amplitude_tri, phase_tri, duration_tri, sampling_rate_tri,
#     frequency_sine, amplitude_sine, phase_sine, duration_sine, sampling_rate_sine)

# 三角+三角
# generate_and_plot_signals(
#      frequency_tri, amplitude_tri, phase_tri, duration_tri, sampling_rate_tri,
#      frequency_tri2, amplitude_tri2, phase_tri2, duration_tri2, sampling_rate_tri2)

# 正弦+正弦
generate_and_plot_signals(
    frequency_sine, amplitude_sine, phase_sine, duration_sine, sampling_rate_sine,
    frequency_sine2, amplitude_sine2, phase_sine2, duration_sine2, sampling_rate_sine2)
