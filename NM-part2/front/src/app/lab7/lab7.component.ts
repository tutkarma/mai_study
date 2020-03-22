import { Component, OnInit } from '@angular/core';
import { ApiClientService } from '../api-client.service';

declare var Plotly: any;


@Component({
  selector: 'app-lab7',
  templateUrl: './lab7.component.html',
  styleUrls: ['./lab7.component.css']
})
export class Lab7Component implements OnInit {

  method = 'leibmann';
  dataNumerical: any
  dataAnalytic: any

  constructor(private apiClient: ApiClientService) {
  }

   solve(N, l, eps) {
      const data = {
          N: N,
          l: l,
          eps: eps,
          equation_type: this.method
      };
      this.apiClient.executeSolver(7, data)
                    .subscribe(data => {
                      console.log(data);
                      this.dataNumerical = data['numerical'];
                      this.dataAnalytic = data['analytic'];
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

      const layoutAnalytic = {
          title: 'analytic',
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
      const dataPlotAnalytic = [{
          z: this.dataAnalytic,
          type: 'surface'
      }];
      Plotly.newPlot('plotAnalytic', dataPlotAnalytic, layoutAnalytic);
  }

  ngOnInit(): void {
  }

}
