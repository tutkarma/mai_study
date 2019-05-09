var cy;

$(function(){
    cy = cytoscape({
        container: document.getElementById('cy'),
        ready: function(){
        },
        layout: {
            name: 'grid',
            rows: 2,
            cols: 2
        },
        style: [{
            selector: 'node',
            style: {
                shape: 'round',
                'height': 50,
                'width': 50,
                'background-color': '#808000',
                'background-fit': 'cover',
                'border-color': '#000',
                'border-width': 3,
                'border-opacity': 0.5,
                label: 'data(id)',
                'text-valign': 'center',
                'color': 'white',
                'text-outline-width': 1,
                'text-outline-color': 'black'
            }
        },
        {
            selector: 'edge',
            style: {
                'curve-style': 'bezier',
                'target-arrow-shape': 'triangle'
            } 
        },
        {
            selector: '.edgehandles-hover',
            css: {
                'background-color': 'red'
            }
        },
        {
            selector: '.edgehandles-source',
            css: {
                'border-width': 2,
                'border-color': 'red'
            }
        },
        {
            selector: '.edgehandles-target',
            css: {
                'border-width': 2,
                'border-color': 'red'
            }
        },
        {
            selector: '.edgehandles-preview, .edgehandles-ghost-edge',
            css: {
                'line-color': 'red',
                'target-arrow-color': 'red',
                'source-arrow-color': 'red'
            }
        }
      ]
  });

  var weight = 0;
  var graph = {};

  cy.edgehandles({
      toggleOffOnLeave: true,
      handleNodes: "node",
      handleSize: 10,
      edgeType: function(){ return 'flat'; },
      complete: function( sourceNode, targetNodes, addedEntities ) {
          var is_not_in_graph = 1;
          weight = Number(prompt("Введите вес ребра:", ''));
          let src = Number(sourceNode.id());
          let trgt = Number(targetNodes.id());
          for(let key in graph) { 
              if(key == src) {
                  let dict_nbr = graph[key];
                  dict_nbr[trgt] = weight;
                  graph[key] = 0; 
                  graph[key] = dict_nbr;
                  is_not_in_graph = 0;
                  break;
              }
          }
          if(is_not_in_graph) {
              let dict_nbr = {};
              dict_nbr[trgt] = weight;
              graph[src] = dict_nbr;
          }
      }
  });

  var i = 0;
  var del_elem = [];


  cy.on('cxttap', 'node' ,function (evt) { //!!!
      cy.remove(cy.$("#" + this.id()));
      del_elem[del_elem.length] = this.id();
  });

  cy.on('cxttap', 'edge' ,function (evt) {
      cy.remove(cy.$("#" + this.id()));

  });

  $('.cytosc').on('tap', '*',function(e) {
      if(del_elem.length == 0) {
          i += 1;
          index = i;
      } else {
          del_elem = del_elem.sort();
          index = del_elem[0];
          del_elem.splice(0,1);
      }
      cy.add([{
          group : "nodes",
          data : {
              id : index
          },
          renderedPosition : {
              x: e.pageX,
              y: e.pageY
          }

      }]);
  });

  var $config = $('#config');
  var $btnParam = $('<div class="param"></div>');
  $config.append( $btnParam );

  var buttons = [
    {
        label: 'Масштаб',
        id :'button_center'
    },
    {
        label: 'Очистка',
        id :'button_remove'
    },
    {
        label: 'Пуск',
        id : 'button_result'
    }
  ];
buttons.forEach( makeButton );

function makeButton( opts ){
    var $button = $('<button class="btn btn-default">'+ opts.label +'</button>');
    $btnParam.append( $button );
    $button.on('click', function(){
        if(opts.id == 'button_center') {
            cy.center();
            cy.fit();
        } else if(opts.id == 'button_remove'){
            cy.remove('*');
            i = 0;
            del_elem = [];
            graph = {};
        } else if(opts.id =='button_result') {
            //alert(graph.toSource());
            var start_node = Number(prompt("Введите начальную вершину:", ''));
            var end_node = Number(prompt("Введите конечную вершину:", ''));
        
            let data = { "graph": graph,
                         "startNode": start_node,
                         "endNode": end_node };
            //alert(data.toSource());

            var data_json = JSON.stringify(data);
            //alert(data_json);

            try{
                $.ajax({
                    type: "POST",
                    url: '_find',
                    data: data_json,
                    contentType: 'application/json;charset=UTF-8',
                    success: function(msg) {
                        var path = $.parseJSON(msg);
                        alert(path);
                    }
                });
            } catch(err) {}
        }
    if( opts.fn ){ opts.fn(); }
    });
}

cy.nodes().forEach(function(n){
    var g = n.data('name');
    n.qtip({
        position: {
            my: 'top center',
            at: 'bottom center'
        },
        style: {
            classes: 'qtip-bootstrap',
            tip: {
                width: 16,
                height: 8
            }
        }
    });
});

$('#config-toggle').on('click', function(){
    $('body').toggleClass('config-closed');
    cy.resize();
  });
});

$(function() {
  FastClick.attach( document.body );
});