<template>
    <section id="container">
        <header>
            <div class="ui stackable grid container">
                <div class="sixteen wide stretched column" style="text-align: center;">
                    Automated Water Monitor
                </div>
            </div>
        </header>

        <section id="main">
            <div class="ui doubling grid container">
                <div class="six wide stretched column">
                    <div class="app wrapper">
                        <h2 class="ui header">
                            <img src="/images/thermometer.svg">
                            <span class="content">
                                Temperature
                                <span class="sub header">Monitor and Control Water Temperature</span>
                            </span>
                        </h2>

                        <div class="temperature slider">
                            <div class="app wrapper">
                                <circle-slider class="app center aligned" :side="150" v-model="preferredTemp" progress-color="#2e86ab" knob-color="#2e86ab" :min="10" :max="55"></circle-slider>

                                <h3 class="ui header center aligned">
                                    Preferred Temperature: <span class="animated color" :style="{color: getTemperatureColor(preferredTemp)}">{{ preferredTemp }} &deg;C</span>
                                </h3>

                                <h3 class="ui header center aligned" style="margin-top: 0;">
                                    Current Temperature: <span class="animated color" :style="{color: getTemperatureColor(variables.current_temperature.value)}">{{ variables.current_temperature.value }} &deg;C</span>
                                </h3>
                            </div>
                        </div>

                        <h2 class="ui header">
                            <img src="/images/pump.svg">
                            <span class="content">
                                Pump Status
                                <span class="sub header">
                                    The pump is currently turned <strong>{{ variables.pump.value }}</strong>.<br>
                                    Current water level is <strong>{{ variables.water_level.value }}%</strong>.
                                </span>
                            </span>
                        </h2>

                        <h2 class="ui header">
                            <img src="/images/heater.svg">
                            <span class="content">
                                Heater Status
                                <span class="sub header">The heater is currently turned <strong>{{ variables.heater.value }}</strong>.</span>
                            </span>
                        </h2>

                        <h2 class="ui header">
                            <img src="/images/freezer.svg">
                            <span class="content">
                                Cooler Status
                                <span class="sub header">The cooler is currently turned <strong>{{ variables.cooler.value }}</strong>.</span>
                            </span>
                        </h2>
                    </div>
                </div>

                <div class="ten wide stretched column">
                    <div class="app wrapper">
                        <h2 class="ui header">
                            <img src="/images/graph.svg">
                            <span class="content">
                                Statistics
                                <span class="sub header">Just because charts look nice</span>
                            </span>
                        </h2>

                        <chartjs-line :data="levels"
                                      :labels="labels"
                                      :datalabel="'Water Level'"
                                      :fill="true"
                                      :option="chartOptions" v-if="levels.length"></chartjs-line>
                    </div>
                </div>
            </div>
        </section>

        <footer>
            <p class="powered by">Made with <span>&hearts;</span> by TBT.</p>
        </footer>
    </section>
</template>
<script>
    import api from '../api';
    import helpers from '../helpers';
    import moment from 'moment';

    export default {
        data()
        {
            return {
                preferredTemp: 0,

                variables: {
                    preferred_temperature: {},
                    current_temperature: {},
                    water_level: {},
                    pump: {},
                    heater: {},
                    cooler: {},
                },

                levels: [],
                labels: [],

                chartOptions: {
                    scales: {
                        xAxes: [{
                            type: "time",
                            time: {
                                parser: 'MM/DD/YYYY HH:mm',
                                tooltipFormat: 'll HH:mm'
                            },
                            scaleLabel: {
                                display: true,
                                labelString: 'Date'
                            }
                        }, ],
                        yAxes: [{
                            scaleLabel: {
                                display: true,
                                labelString: 'Water Level'
                            },
                            ticks: {
                                beginAtZero: true,
                                max: 100,
                            }
                        }]
                    },
                },
            };
        },

        methods: {
            getTemperatureColor(temp)
            {
                if (temp > 45)
                {
                    return '#ff3f00';
                }

                if (temp > 35)
                {
                    return '#bbdb04';
                }

                return '#3f8efc';
            }
        },

        created()
        {
            api.get('/water-levels', {}, (d) =>
            {
                d.data.forEach((v) =>
                {
                    this.levels.push({
                        x: helpers.moment(v.timestamp).format('MM/DD/YYYY HH:mm'),
                        y: v.level
                    });
                });
            });

            for (let i = 0; i < 24; i++)
            {
                this.labels.push(
                    moment().subtract(24 - i, 'h').toDate()
                );
            }

            setInterval(() =>
            {
                api.get('/variables', {}, (d) =>
                {
                    this.variables = d;

                    if (this.preferredTemp != d.preferred_temperature.value)
                    {
                        api.put('/variables/preferred_temperature', {value: this.preferredTemp});
                    }
                });
            }, 1000);
        },

        beforeCreate()
        {
            api.get('/variables', [], (d) =>
            {
                this.variables = d;
                this.preferredTemp = d.preferred_temperature.value;
            });
        }
    }
</script>