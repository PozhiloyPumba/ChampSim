import os
import json
import numpy as np
import matplotlib.pyplot as plt


BIMODAL_JSON_PATH = './baseline/out/'
GAG_JSON_PATH = './GAg/out/'
GAP_JSON_PATH = './GAp/out/'
PICTURES_PATH = './pictures/'


class TraceStats:
    def __init__(self):
        self.names = []
        self.ipcs = []
        self.mpkis = []

def gmean(iterable):
    return np.exp(np.log(iterable).mean())

def parse_file_stats(filenames):
    infos = TraceStats()

    for filename in filenames:
        with open(filename) as f:
            
            all_stats = json.load(f)
            roi_stats = all_stats[0]['roi']['cores'][0]

            cycles = float(roi_stats['cycles'])
            instructions = float(roi_stats['instructions'])
            mispredict = roi_stats['mispredict']

            # infos.names.append(os.path.basename(filename))
            infos.ipcs.append(instructions / cycles)
            infos.mpkis.append(1000 * sum(mispredict.values()) / instructions)

    return infos


def parse_stats(dirname):
    return parse_file_stats([dirname + x for x in os.listdir(dirname)])


def plot_stats(bimodal_stats, GAg_stats, GAp_stats):
    predictor_names = [
        "Bimodal",
        "GAg",
        "GAp"
    ]

    predictor_ipc_gmean = [
        gmean(bimodal_stats.ipcs),
        gmean(GAg_stats.ipcs),
        gmean(GAp_stats.ipcs)
    ]

    predictor_mpki_gmean = [
        gmean(bimodal_stats.mpkis),
        gmean(GAg_stats.mpkis),
        gmean(GAp_stats.mpkis)
    ]

    fig, ax = plt.subplots(1, 1, figsize=(12, 8))
    fig.suptitle('Instructions Per Cycle', fontsize=20)
    ax.grid(zorder=0)
    ax.set_xlabel('Predictor', fontsize=18)
    ax.set_ylabel('IPC', fontsize=18)
    ax.tick_params(axis='both', which='major', labelsize=14)
    rects = ax.bar(predictor_names, predictor_ipc_gmean, align='center', zorder=2)
    ax.bar_label(rects, padding=2, fontsize=16)
    ax.set_ylim([0.8, 1])
    plt.savefig(PICTURES_PATH + 'ipc_gmean_zoomed.jpg')
    plt.close()

    fig, ax = plt.subplots(1, 1, figsize=(12, 8))
    fig.suptitle('Mispredictions Per Kilo Instruction', fontsize=20)
    ax.grid(zorder=0)
    ax.set_xlabel('Predictor', fontsize=18)
    ax.set_ylabel('MPKI', fontsize=18)
    ax.tick_params(axis='both', which='major', labelsize=14)
    rects = ax.bar(predictor_names, predictor_mpki_gmean, align='center', zorder=2)
    ax.bar_label(rects, padding=2, fontsize=16)
    plt.savefig(PICTURES_PATH + 'mpki_gmean.jpg')
    plt.close()



def main():
    bimodal_stats = parse_stats(BIMODAL_JSON_PATH)
    GAg_stats = parse_stats(GAG_JSON_PATH)
    GAp_stats = parse_stats(GAP_JSON_PATH)

    plot_stats(bimodal_stats, GAg_stats, GAp_stats)


if __name__ == '__main__':
    main()