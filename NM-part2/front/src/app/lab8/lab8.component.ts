import { Component, OnInit } from '@angular/core';
import { ApiClientService } from '../api-client.service';

declare var Plotly: any;

@Component({
  selector: 'app-lab8',
  templateUrl: './lab8.component.html',
  styleUrls: ['./lab8.component.css']
})
export class Lab8Component implements OnInit {

  method = 'alter_directions';
  dataNumerical: any
  dataAnalyticX: any
  dataAnalyticY: any

  constructor(private apiClient: ApiClientService) {
  }

    solve(N1, N2, K, T) {
      const data = {
          N1: N1,
          N2: N2,
          K: K,
          T: T,
          equation_type: this.method
      };
      this.apiClient.executeSolver(8, data)
                    .subscribe(data => {
                      console.log(data);
                      this.dataNumerical = data['numerical'];
                      this.dataAnalyticX = data['analytic']['grid_x'];
                      this.dataAnalyticY = data['analytic']['grid_y'];
      });

      const layoutNumerical = {
          title: this.method,
          autosize: false,
          width: 400,
          height: 400,
          margin: {
            l: 65,
            r: 50,
            b: 65,
            t: 90,
          }
      };
      const dataPlotNumerical = [{
          z: this.dataNumerical,
          type: 'surface'
      }];
      Plotly.newPlot('plotNumerical', dataPlotNumerical, layoutNumerical);

    const dataTraceX = {
      y: this.dataAnalyticX.map(s => this.sum(s) / s.length),
      type: 'scatter',
      name: 'Analytic X'
    };

    const dataTraceY = {
      y: this.dataAnalyticY.map(s => this.sum(s) / s.length),
      type: 'scatter',
      name: 'Analytic Y'
    };

    Plotly.newPlot('solution-comparison-x', [[], dataTraceX]);
    Plotly.newPlot('solution-comparison-y', [[], dataTraceY]);
  }

  sum = arr => arr.reduce((a, b) => a + b, 0);

  ngOnInit(): void {
  }

}
